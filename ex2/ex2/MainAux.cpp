#include "MainAux.h"


/* ------------------------------- Event messages functions ------------------------------- */

void printWinMessage(const int numOfSteps) {
	cout << "Succeeded in " << numOfSteps << " steps" << endl;
}

void printLostMessage(const int numOfSteps) {
	cout << "Failed to solve maze in " << numOfSteps << " steps" << endl;
}

void printWrongArgumentsFormatError()
{
	cout << "Wrong format arguments" << endl;
}

void printBadAlgorithmWarning(const string & algoName)
{
	cout << "Warning: " << algoName << " is invalid." << endl;
}

void printBadMazeWarning(const string & mazeName)
{
	cout << "Warning: " << mazeName << " is invalid." << endl;
}

void printStreamError(const string & filename) {
	cout << "Could not open " << filename << "." << endl;
}

void printPopenError()
{
	cout << "Error: error in popen command." << endl;
}

void printHeaderErrorTitle()
{
	cout << "Bad maze file header:" << endl;
}

void printMazeErrorTitle()
{
	cout << "Bad maze in maze file:" << endl;
}

void printMaxStepsError(const string & str)
{
	cout << "expected in line 2 - MaxSteps = <num>" << endl;
	cout << "got: " << str << endl;
}

void printRowsError(const string & str)
{
	cout << "expected in line 3 - Rows = <num>" << endl;
	cout << "got: " << str << endl;
}

void printColsError(const string & str)
{
	cout << "expected in line 4 - Cols = <num>" << endl;
	cout << "got: " << str << endl;
}

void printMissingPlayerCharError(const string & str)
{
	(void)str;
	cout << "Missing @ in maze" << endl;
}

void printMissingEndCharError(const string & str)
{
	(void)str;
	cout << "Missing $ in maze" << endl;
}

void printMoreThanOnePlayerCharError(const string & str)
{
	(void)str;
	cout << "More than one @ in maze" << endl;
}

void printMoreThanOneEndCharError(const string & str)
{
	(void)str;
	cout << "More than one $ in maze" << endl;
}

void printWrongCharError(const string & str)
{
	if (str[0] == 9)
		cout << "Wrong character in maze: TAB in row " << ((int)str[1] + 1) << ", col " << ((int)str[2] + 1) << endl;
	else
		cout << "Wrong character in maze: " << str[0] << " in row " << ((int)str[1] + 1) << ", col " << ((int)str[2] + 1) << endl;
}





/* ------------------------------- Parsing helper functions ------------------------------ */

/* This function returns true iff <path> is an existing file path. */
bool fileExists(const char* path) {
	struct stat buf;
	return (stat(path, &buf) == 0);
}

/* This function returns true iff <path> is an existing directory. */
bool pathExist(const char* path) {
	struct stat statbuf;
	return (stat(path, &statbuf) != -1 && S_ISDIR(statbuf.st_mode));
}

/* This function returns true iff <mainStr> string ends with <toMatch> string. */
bool endsWith(const string & mainStr, const string & toMatch)
{
	return mainStr.size() >= toMatch.size() &&
		mainStr.compare(mainStr.size() - toMatch.size(), toMatch.size(), toMatch) == 0;
}

/*	This function fills <argVec> string vector with the entered arguments,
	and returns true iff the program's arguments are valid according to the guidelines. */
bool initArgumentVector(int argc, char * argv[], vector<string>& argVec) {
	vector<bool> argHasBeenGiven = { false, false, false };
	bool validArgs = true;
	switch (argc) {
	case 9:
		parsePairOfArguments(argv[7], argv[8], validArgs, argVec, argHasBeenGiven);
	case 7:
		parsePairOfArguments(argv[5], argv[6], validArgs, argVec, argHasBeenGiven);
	case 5:
		parsePairOfArguments(argv[3], argv[4], validArgs, argVec, argHasBeenGiven);
	case 3:
		parsePairOfArguments(argv[1], argv[2], validArgs, argVec, argHasBeenGiven);
	case 1:
		break;
	default:
		validArgs = false;
		break;
	}
	return validArgs;
}

/*	This function indicates what is <arg> are according to the guidelines:
	A maze path, an algorithm path, an output path or an integer indicates #threads.
	Accordingly, it puts <arg> in it's right position in <argVec> vector.  */
void parsePairOfArguments(char * argType, char * arg, bool & validArgs, vector<string>& argVec, vector<bool>& argHasBeenGiven) {

	if (strcmp(argType, "-maze_path") == 0 && pathExist(arg)) { // .maze folder path
		if (!argHasBeenGiven[MAZEPATH_INDEX]) {
			argHasBeenGiven[MAZEPATH_INDEX] = true;
			argVec[MAZEPATH_INDEX] = arg;
		}
		else validArgs = false;
	}
	else if (strcmp(argType, "-algorithm_path") == 0 && pathExist(arg)) { // .so folder path
		if (!argHasBeenGiven[ALGOPATH_INDEX]) {
			argHasBeenGiven[ALGOPATH_INDEX] = true;
			argVec[ALGOPATH_INDEX] = arg;
		}
		else validArgs = false;
	}
	else if (strcmp(argType, "-output") == 0 && pathExist(arg)) { // .output folder path
		if (!argHasBeenGiven[OUTPUTPATH_INDEX]) {
			argHasBeenGiven[OUTPUTPATH_INDEX] = true;
			argVec[OUTPUTPATH_INDEX] = arg;
		}
		else validArgs = false;
	}
	else if (strcmp(argType, "-num_threads") == 0 && atoi(arg) != 0) { // .output folder path
		if (!argHasBeenGiven[NUMTHREADS_INDEX]) {
			argHasBeenGiven[NUMTHREADS_INDEX] = true;
			argVec[NUMTHREADS_INDEX] = arg;
		}
		else validArgs = false;
	}
	else validArgs = false;
}





/* -------------------------- GameManager play() helper functions ------------------------ */

/* This function retrieves the capital letter representing the Move <a>. */
char getMoveChar(const Move& a) {
	switch (a) {
	case Move::UP:
		return 'U';
	case Move::DOWN:
		return 'D';
	case Move::LEFT:
		return 'L';
	case Move::RIGHT:
		return 'R';
	default:
		return 'B';
	}
}

/* This function returns the opposite direction of Move <a> */
Move operator!(const Move& a) {
	switch (a) {
	case Move::UP:
		return Move::DOWN;
	case Move::DOWN:
		return Move::UP;
	case Move::LEFT:
		return Move::RIGHT;
	case Move::RIGHT:
		return Move::LEFT;
	default:
		return a;
	}
}

void updateCoordinate(Coordinate & c, const int i, const int j) {
	c.first = i;
	c.second = j;
}




/* --------------------------- output creation helper functions --------------------------- */

/*	A helper function for FileHandler::createOutput().
	params: an integer <num_of_mazes>.
	This function prints a seperation row for the output table. */
void printSeperationRow(const unsigned int num_of_mazes) {
	for (unsigned int i = 0; i < (TABLE_COLUMN_LENGTH + 1) * (num_of_mazes + 1); i++)
		cout << "-";
	cout << endl;
}

/*	A helper function for FileHandler::createOutput().
	params: an integer <num_of_mazes>.
	This function prints a title for the output table, filled with all the mazes names. */
void printTitles(const unsigned int num_of_mazes, const vector<string>& mazeNameVector) {
	cout << "|";
	for (unsigned int i = 0; i < TABLE_COLUMN_LENGTH; i++)
		cout << " ";
	for (unsigned int j = 0; j < num_of_mazes; j++) {
		cout << "|" << mazeNameVector[j];
		for (unsigned int i = 0; i < TABLE_COLUMN_LENGTH - mazeNameVector[j].length(); i++)
			cout << " ";
	}
	cout << "|" << endl;
}

/*	A helper function for FileHandler::createOutput().
	params: an algorithm name <algoName>.
	This function algoName at every row beginning in the output table. */
void printAlgorithmName(const string & algoName) {
	cout << "|" << algoName;
	for (unsigned int j = 0; j < TABLE_COLUMN_LENGTH - algoName.length(); j++) {
		cout << " ";
	}
}

/*	A helper function for FileHandler::createOutput().
	params: an integer <num_of_mazes>, an algorithm index <algoIndex> and an algorithm name <algoName>.
	This function prints a line of the results for algoName running on each maze in the output table,
	and creates an output file for each maze (if outputPath exists). */
void printAlgorithmResultOnAllMazes(const string & path, const unsigned int num_of_mazes,
									const string & algoName, const vector<MoveListMap> & allMatchesMoveListMaps,
									const vector<string> & mazeNameVector) {
	for (unsigned int j = 0; j < num_of_mazes; j++) {
		MoveListMap map = allMatchesMoveListMaps[j];
		MoveList lst = map[algoName];
		cout << "|";
		if (lst[lst.size() - 1] == '!') {
			string str = to_string(lst.size() - 1);
			for (unsigned int k = 0; k < TABLE_COLUMN_LENGTH - str.length(); k++)
				cout << " ";
			cout << str;
		}
		else {
			for (unsigned int k = 0; k < TABLE_COLUMN_LENGTH - 2; k++)
				cout << " ";
			cout << "-1";
		}
		// output file handling
		if (outputPathExists(path)) createOutputFile(path, algoName, mazeNameVector[j], lst);
	}
	cout << "|" << endl;
}

/*	A helper function for printAlgorithmResultOnAllMazes().
	params: an algorithm name, a maze name and a move list.
	This function creates an ofstream and pushes the move chars into it. */
void createOutputFile(const string& path, const string & algoName, const string & mazeName, const MoveList& moveList) {
	ofstream fout = ofstream();
	string filename = getAvaliableFileName(path, algoName, mazeName);
	fout.open(filename);
	if (!fout.is_open()) {
		printStreamError(filename);
		return;
	}
	pushMovesToOutputFile(fout, moveList);
	fout.close();
}

/*	A helper function for createOutputFile().
	params: an open ofstream and a vector of game moves.
	This function pushes the move chars into the output stream. */
void pushMovesToOutputFile(ofstream & fout, const MoveList& moveList) {
	for (const char & c : moveList)
		fout << c << endl;
}

/*	A helper function for createOutputFile().
	Returns a valid, non-taken output file path. */
string getAvaliableFileName(const string& path, const string & algoName, const string & mazeName) {
	string filename = path + "/" + mazeName + "_" + algoName + ".output";
	int count = 1;
	while (fileExists(filename.c_str())) {
		filename = path + "/" + mazeName + "_" + algoName + "(" + to_string(count) + ").output";
		count++;
	}
	return filename;
}

