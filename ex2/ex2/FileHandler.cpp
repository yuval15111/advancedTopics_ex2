#include "FileHandler.h"
#include <algorithm>

/* -------------- maze and so files searching and reading functions ----------------- */

/* This function opens a pipe contains the output of cmd command. */
FILE * FileHandler::execCmd(const char * cmd) {
	FILE * dl = popen(cmd, "r");
	if (!dl) {
		printPopenError();
		return NULL;
	}
	return dl;
}

/* This function opens an ifstream for a maze file to be read. */
unique_ptr<ifstream> FileHandler::openIFstream(const char * filename) {
	unique_ptr<ifstream> fin = make_unique<ifstream>(filename);
	cout << "made fin successfully" << endl;
	if (!(*fin).is_open()) {
		printStreamError(filename);
		return nullptr;
	}
	cout << "return fin: " << endl;
	return fin;
}

/*	This function iterates over a list of .so files in the correct format
	in a pipe <dl>, and parses them into algorithms. */
void FileHandler::generateAlgorithmsFromSoFiles(FILE * dl) {
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
				continue; // bad so file - keep looking for other files
			}
			m_dlVector.emplace_back(dlib); // to be closed
			m_algorithmNameVector.push_back(algorithmName);
		}
	}
}

/*	This function iterates over a list of .maze files in the correct format
	in a pipe <dl>, and parses them into MatchManager objects (each represents a maze). */
void FileHandler::generateMatchesFromMazeFiles(FILE * dl) {
	char in_buf[BUF_SIZE];
	while (fgets(in_buf, BUF_SIZE, dl)) {
		// trim off the whitespace
		char* ws = strpbrk(in_buf, " \t\n");
		if (ws) *ws = '\0';
		string filename(in_buf);
		cout << "fin begin: " << endl;
		unique_ptr<ifstream> fin = openIFstream(filename.c_str());
		cout << "mm begin using move(fin): " << endl;
		unique_ptr<MatchManager> mm = parseMaze(move(fin));
		(*fin).close();
		if (mm == nullptr) {
			printBadMazeWarning(filename);
			continue; // bad maze - keep looking for other mazes
		}
		cout << "mm gameManager creation begin: " << endl;
		mm->createGameManagers();
		cout << "mm putting in matchVector begin: " << endl;
		m_matchVector.emplace_back(move(mm));
	}
}

/* ------------------------------ maze parsing functions ---------------------------- */


/*	This is the maze parsing function, which parses an input stream
	and creates a MatchManager object. */
unique_ptr<MatchManager> FileHandler::parseMaze(unique_ptr<ifstream> fin) {
	m_errors.no_parsing_Errors = true;
	string line;

	// Collect maze parameters:
	string name = getName(move(fin), line);
	int maxSteps = getIntValue(move(fin), MAXSTEPS, ErrorType::MaxStepsError, line);
	int rowsNum = getIntValue(move(fin), ROWS, ErrorType::RowsError, line);
	int colsNum = getIntValue(move(fin), COLS, ErrorType::ColsError, line);

	// Check errors in lines 2-4:
	checkErrors((void*)printHeaderErrorTitle);

	if (m_errors.no_parsing_Errors) {	// No errors, lines 2-4 are valid.
		Coordinate playerLocation, endLocation;
		// Collect other maze properties
		MazeBoard board = getBoard(move(fin), rowsNum, colsNum, playerLocation, endLocation, line);

		// Check errors in the maze itself:
		checkErrors((void*)printMazeErrorTitle);
		cout << "return mm begin: " << endl;
		if (m_errors.no_parsing_Errors)	// No errors, maze file is valid - creates a MatchManager object
			return make_unique<MatchManager>(name, maxSteps, rowsNum, colsNum,
				board, playerLocation, endLocation, m_algorithmNameVector, m_numOfThreads);
	}

	return nullptr;
}

/*	A helper function for parseMaze().
	This function retrieves the name of the maze. */
string FileHandler::getName(unique_ptr<ifstream> fin, string & line) {
	if (getline(*fin, line)) {
		return line;
	}
	return nullptr;
}

/*	A helper function for parseMaze().
	This function retrieves the integer value from lines 2-4. */
int FileHandler::getIntValue(unique_ptr<ifstream> fin, const string & input, const ErrorType error, string & line) {
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

/*	A helper function for parseMaze().
	params: rows, col - parsed from maze file;
			references to playerLocation and endLocation that will be filled in this function;
			refernce to line string which we fill with lines from the input and parse the file with.
	return: A maze board object (two-dimensional character vector) */
MazeBoard FileHandler::getBoard(unique_ptr<ifstream> fin, const int rows, const int cols, Coordinate & playerLocation, Coordinate & endLocation, string & line) {
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

/*	A helper function for parseMaze().
	This function checks if there are errors. If so: updates m_errors.no_parsing_Errors field
	to false and prints the errors. */
void FileHandler::checkErrors(void*(titleFunc)) {
	if (m_errors.list.size() == 0) return;
	if (titleFunc != nullptr) { // parsing errors exist
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

/* -------------------------------- other helper functions -------------------------------- */

/* This function retrieves a vector of all the mazes' names. */
vector<string> FileHandler::getMazeNamesVector() {
	vector<string> vec;
	for (unsigned int i = 0; i < m_matchVector.size(); i++)
		vec.emplace_back(m_matchVector[i]->getName());
	return vec;
}

/* This function retrieves a vector of all the mazes' MoveLists objects. */
vector<MatchMoveLists> FileHandler::getAllMatchesMoveLists() {
	vector<MatchMoveLists> vec;
	for (unsigned int i = 0; i < m_matchVector.size(); i++)
		vec.emplace_back(m_matchVector[i]->getMoveListVector());
	return vec;
}

/* ---------------------------------------------------------------------------------------- */
/* ----------------------------- File Handler public functions ---------------------------- */
/* ---------------------------------------------------------------------------------------- */

/*	the d'tor is responsible for deleting every memory allocation that a FileHandler object  
	has allocated during its life: factoryMethods, MatchManagers and dls. */
FileHandler::~FileHandler() {
	AlgorithmRegistrar::getInstance().clearVector();	// factoryMethod deletion
	for (void * dl : m_dlVector)
		if (dl != NULL)	dlclose(dl);					// dl objects deletion
}

/*	This function seeks for all the .so files from <m_algorithmPath> and generates
	a maze solver algorithm from each of them. */
void FileHandler::getAlgorithms() {
	FILE* dl;  // handle to read directory 
	string command_str = "ls " + m_algorithmPath + "/*.so";
	if ((dl = execCmd(command_str.c_str())) == NULL) return;
	generateAlgorithmsFromSoFiles(dl);
	pclose(dl);
}

/*	This function seeks for all the .maze files from <m_mazePath> and generates
	a maze match manager from each of them. */
void FileHandler::getMatchesAndPlay() {
	FILE* dl;  // handle to read directory 
	string command_str = "ls " + m_mazePath + "/*.maze";
	if ((dl = execCmd(command_str.c_str())) == NULL) return;
	generateMatchesFromMazeFiles(dl);
	pclose(dl);
}

/*	This function is responsible for writing the output table to the screen, and, if 
	an output path is given by user, creating an output files. */
void FileHandler::createOutput() {
	if (m_algorithmNameVector.size() == 0 || m_matchVector.size() == 0) return; // nothing to do here
	vector<string> mazeNamesVec = getMazeNamesVector();
	unsigned int num_of_mazes = m_matchVector.size();
	printSeperationRow(num_of_mazes);
	printTitles(num_of_mazes, mazeNamesVec); // Table title

	// Informative table rows:
	for (unsigned int i = 0; i < m_algorithmNameVector.size(); i++) {
		printSeperationRow(num_of_mazes);
		string & algoName = m_algorithmNameVector[i];
		printAlgorithmName(algoName);
		printAlgorithmResultOnAllMazes(m_outputPath, num_of_mazes, i, algoName, getAllMatchesMoveLists(), mazeNamesVec);
	
	}
	printSeperationRow(num_of_mazes);
}