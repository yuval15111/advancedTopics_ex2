#include "FileHandler.h"
#include <algorithm>

FileHandler::FileHandler(vector<string>& paths) {
	m_mazePath = paths[MAZEPATH_INDEX];
	m_algorithmPath = paths[ALGOPATH_INDEX];
	m_outputPath = paths[OUTPUTPATH_INDEX];
}

FileHandler::~FileHandler() {
	AlgorithmRegistrar::getInstance().clearVector();

	for (MatchManager * mm : m_matchVector)
		if (mm != nullptr) delete mm;

	for (void * dl : dlVector)
		if (dl != NULL)	dlclose(dl);
}

FILE * FileHandler::execCmd(const char * cmd) {
	FILE * dl = popen(cmd, "r");
	if (!dl) {
		printPopenError();
		return NULL;
	}
	return dl;
}

ifstream * FileHandler::openIFstream(const char * filename) {
	ifstream *fin = new ifstream(filename);
	if (!(*fin).is_open()) {
		printStreamError(filename);
		return nullptr;
	}
	return fin;
}

void FileHandler::iterateOverMazeFiles(FILE * dl) {
	char in_buf[BUF_SIZE];
	while (fgets(in_buf, BUF_SIZE, dl)) {
		// trim off the whitespace
		char* ws = strpbrk(in_buf, " \t\n");
		if (ws) *ws = '\0';
		string filename(in_buf);
		ifstream *fin = openIFstream(filename.c_str());
		MatchManager * mm = parseMaze(fin);
		(*fin).close();
		delete fin;
		if (mm == nullptr) {
			printBadMazeWarning(filename);
			continue; // bad maze - keep looking for other mazes
		}
		mm->createGameManagers();
		m_matchVector.push_back(mm);
	}
}

void FileHandler::getMatches() {
	FILE* dl;  // handle to read directory 
	string command_str = "ls " + m_mazePath + "/*.maze";
	if ((dl = execCmd(command_str.c_str())) == NULL) return;
	iterateOverMazeFiles(dl);
	pclose(dl);
}


void FileHandler::iterateOverSOFiles(FILE * dl) {
	void* dlib;
	char in_buf[BUF_SIZE];
	while (fgets(in_buf, BUF_SIZE, dl)) {
		// trim off the whitespace
		char* ws = strpbrk(in_buf, " \t\n");
		if (ws) *ws = '\0';
		string filename(in_buf);
		size_t index = filename.find("_308243351_");
		if ((index != string::npos) && (endsWith(filename, ".so"))) {
			string algorithmName = filename.substr(index, 12);
			dlib = dlopen(filename.c_str(), RTLD_LAZY);
			if (dlib == NULL) {
				printBadAlgorithmWarning(algorithmName);
				continue;
			}
			dlVector.emplace_back(dlib); // to be closed
			m_algorithmNameVector.push_back(algorithmName);
		}
	}
}

void FileHandler::getAlgorithms() {
	FILE* dl;  // handle to read directory 
	string command_str = "ls " + m_algorithmPath + "/*.so";
	if ((dl = execCmd(command_str.c_str())) == NULL) return;
	iterateOverSOFiles(dl);
	pclose(dl);
}

void FileHandler::printSeperationRow(unsigned int num_of_mazes) {
	for (unsigned int i = 0; i < (COLUMN_LENGTH + 1) * (num_of_mazes + 1); i++)
		cout << "-";
	cout << endl;
}

void FileHandler::printTitles(unsigned int num_of_mazes) {
	// first column - algorithms title
	cout << "|";
	for (unsigned int i = 0; i < COLUMN_LENGTH; i++)
		cout << " ";

	// rest of the columns - mazes titles
	for (unsigned int j = 0; j < num_of_mazes; j++) {
		string & mazeName = m_matchVector[j]->getName();
		cout << "|" << mazeName;
		for (unsigned int i = 0; i < COLUMN_LENGTH - mazeName.length(); i++) {
			cout << " ";
		}
	}
	cout << "|" << endl;
}

void FileHandler::printAlgorithmName(string & algoName)
{
	cout << "|" << algoName;
	for (unsigned int j = 0; j < COLUMN_LENGTH - algoName.length(); j++) {
		cout << " ";
	}
}

void FileHandler::printAlgorithmResultOnAllMazes(unsigned int i, string & algoName) {
	for (unsigned int j = 0; j < m_matchVector.size(); j++) {
		vector<vector<char>> moveListVector = m_matchVector[j]->getMoveListVector();
		cout << "|";
		if (moveListVector[i][moveListVector[i].size() - 1] == '!') {
			string str = to_string(moveListVector[i].size() - 1);
			for (unsigned int k = 0; k < COLUMN_LENGTH - str.length(); k++) {
				cout << " ";
			}
			cout << str;
		}
		else {
			for (unsigned int k = 0; k < COLUMN_LENGTH - 2; k++) {
				cout << " ";
			}
			cout << "-1";
		}

		// output file handling
		if (outputPathExists()) {
			string & mazeName = m_matchVector[j]->getName();
			createOutputFile(algoName, mazeName, moveListVector[i]);
		}
	}
	cout << "|" << endl;
}

void FileHandler::createOutputFile(string & algoName, string & mazeName, vector<char>& moveList) {
	ofstream fout = ofstream();
	string filename = getAvaliableFileName(algoName, mazeName);
	fout.open(filename);
	if (!fout.is_open()) {
		printStreamError(filename);
		return;
	}
	pushMovesToOutputFile(fout, moveList);
	fout.close();
}

/*	params: vector of game actions.
	This function pushes the actions vector into the output file. */
void FileHandler::pushMovesToOutputFile(ofstream & fout, vector<char>& moveList) {
	for (const char & c : moveList)
		fout << c << endl;
}

string FileHandler::getAvaliableFileName(string & algoName, string & mazeName) {
	string filename = m_outputPath + "/" + mazeName + "_" + algoName + ".output";
	int count = 1;
	while (fileExists(filename.c_str())) {
		filename = m_outputPath + "/" + mazeName + "_" + algoName + "(" + to_string(count) + ").output";
		count++;
	}
	return filename;
}

void FileHandler::createOutput() {
	if (m_algorithmNameVector.size() == 0 || m_matchVector.size() == 0) return; // nothing to do here
	unsigned int num_of_mazes = m_matchVector.size();
	printSeperationRow(num_of_mazes);
	printTitles( num_of_mazes);

	// information rows
	for (unsigned int i = 0; i < m_algorithmNameVector.size(); i++) {
		printSeperationRow(num_of_mazes);
		string & algoName = m_algorithmNameVector[i];
		printAlgorithmName(algoName);
		printAlgorithmResultOnAllMazes(i, algoName);
	}
	printSeperationRow(num_of_mazes);
}

/*	This function checks if there are errors. If so: updates m_errors.noErrors field to false and prints the errors */
void FileHandler::checkErrors(void*(titleFunc)) {
	if (m_errors.list.size() == 0) return;
	if (titleFunc != nullptr) { // parsing errors
		FuncNoArgs f = (FuncNoArgs)titleFunc;
		f();
		m_errors.no_parsing_Errors = false;
	}
	for (ErrorList::iterator it = m_errors.list.begin(); it != m_errors.list.end(); ++it) {
		Func f = m_errors.fmap[it->first];
		string str = it->second;
		f(str);
	}
	m_errors.list.clear();
}

/* This function parses the input file and creates the manager object. */
MatchManager * FileHandler::parseMaze(ifstream * fin) {
	m_errors.no_parsing_Errors = true;
	string line;
	string name = getName(fin, line);															// Collects maze parameters
	int maxSteps = getIntValue(fin, MAXSTEPS, ErrorType::MaxStepsError, line);
	int rowsNum = getIntValue(fin, ROWS, ErrorType::RowsError, line);
	int colsNum = getIntValue(fin, COLS, ErrorType::ColsError, line);
	checkErrors((void*)printHeaderErrorTitle);
	if (m_errors.no_parsing_Errors) {														// No errors, lines 2-4 are valid.
		Coordinate playerLocation, endLocation;
		MazeBoard board = getBoard(fin, rowsNum, colsNum, playerLocation, endLocation, line);
		checkErrors((void*)printMazeErrorTitle);
		if (m_errors.no_parsing_Errors)							// No errors, maze file is valid - creates a Manager object
			return new MatchManager(name, maxSteps, rowsNum, colsNum,
				board, playerLocation, endLocation, m_algorithmNameVector);
	}
	return nullptr;
}

/* This function retrieves the name of the maze. */
string FileHandler::getName(ifstream * fin, string & line) {
	if (getline(*fin, line)) {
		return line;
	}
	return nullptr;
}

/* This function retrieves the integer value for lines 2-4. */
int FileHandler::getIntValue(ifstream * fin, const string & input, const ErrorType error, string & line) {
	const regex reg("\\s*" + input + "\\s*=\\s*[1-9][0-9]*\\s*$");

	const regex numReg("[1-9][0-9]*");
	smatch match;
	if (getline(*fin, line)) {
		if (!regex_match(line, reg)) {
			pushError(error, line);
			return -1;
		}
		regex_search(line, match, numReg);
		return stoi(match[0]);
	}
	pushError(error, line);
	return -1;
}

/*	params: rows, col - parsed from maze file; references to playerLocation and endLocation that will be filled in this function;
			refernce to line string which we fill with lines from the input and parse the file with.
	return: A maze board object (two-dimensional character vector) */
MazeBoard FileHandler::getBoard(ifstream * fin, const int rows, const int cols, Coordinate & playerLocation, Coordinate & endLocation, string & line) {
	MazeBoard board;
	bool seenPlayerChar = false, seenEndChar = false;
	for (int i = 0; i < rows; i++) {
		MazeRow row;
		if (getline(*fin, line)) {											// Succeeded reading a line - fills MazeRow according to line
			for (int j = 0; j < min(cols, (int)line.length()); j++) {
				if (line[j] == PLAYER_CHAR)
					handleSpecialChar(PLAYER_CHAR, playerLocation, i, j, seenPlayerChar, line, ErrorType::MoreThanOnePlayerChar);
				else if (line[j] == END_CHAR)
					handleSpecialChar(END_CHAR, endLocation, i, j, seenEndChar, line, ErrorType::MoreThanOneEndChar);
				else if (line[j] == '\r') line[j] = SPACE_CHAR;
				else if (line[j] != SPACE_CHAR && line[j] != WALL_CHAR) // other chars are invalid
					handleInvalidChar(line[j], i, j);
				row.push_back(line[j]);
			}
			for (int j = (int)line.length(); j < cols; j++)					// Fill with SPACE_CHAR when cols > line.length()
				row.push_back(SPACE_CHAR);
		}
		else																// No more lines in maze file - fills the row with SPACE_CHARs
			for (int j = 0; j < cols; j++)
				row.push_back(SPACE_CHAR);
		board.push_back(row);
	}
	if (!seenPlayerChar) pushError(ErrorType::MissingPlayerChar, string());
	if (!seenEndChar) pushError(ErrorType::MissingEndChar, string());
	return board;
}

/*	A helper function for getBoard().
	Params: char c, location reference, (i, j) new coordinate indices, other helping parameters.
	The function updates the location coordinate by (i, j) values or pushes errors to the Errors vector if needed. */
void FileHandler::handleSpecialChar(const char c, Coordinate & location, const int i, const int j, bool & seenChar, string & line, const ErrorType e) {
	if (!seenChar) {
		updateCoordinate(location, i, j);
		seenChar = true;
		if (c == PLAYER_CHAR) line[j] = SPACE_CHAR; // not necessary as '@' character
	}
	else { // only one player char allowed!
		pushError(e, string());
	}
}

/*	A helper function for getBoard().
	Params: invalid char c, (i, j) error indices.
	The function pushes invalid char error to Error vector. */
void FileHandler::handleInvalidChar(const char c, const int i, const int j) {
	string str = "000";
	str[0] = c;
	str[1] = (char)i;
	str[2] = (char)j;
	pushError(ErrorType::WrongChar, str);
}