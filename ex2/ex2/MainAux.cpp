#include "MainAux.h"

/* return: the opposite direction of Move a */
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

void printBadAlgorithmWarning(string algoName)
{
	cout << "Warning: " << algoName << " is invalid." << endl;
}

void printBadMazeWarning(string mazeName)
{
	cout << "Warning: " << mazeName << " is invalid." << endl;
}

void printStreamError(string filename) {
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

/* return true iff the file in path exists */
bool fileExists(const char* path) {
	struct stat buf;
	return (stat(path, &buf) == 0);
}

bool pathExist(const char* path) {
	struct stat statbuf;
	return (stat(path, &statbuf) != -1 && S_ISDIR(statbuf.st_mode));
}

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

void updateCoordinate(Coordinate & c, const int i, const int j) {
	c.first = i;
	c.second = j;
}

bool endsWith(const string & mainStr, const string & toMatch)
{
	return mainStr.size() >= toMatch.size() &&
		mainStr.compare(mainStr.size() - toMatch.size(), toMatch.size(), toMatch) == 0;
}

bool initPaths(int argc, char * argv[], vector<string>& paths) {
	vector<bool> pathHasBeenGiven = { false, false, false };
	bool validArgs = true;
	switch (argc) {
	case 7:
		parsePairOfArguments(argv[5], argv[6], validArgs, paths, pathHasBeenGiven);
	case 5:
		parsePairOfArguments(argv[3], argv[4], validArgs, paths, pathHasBeenGiven);
	case 3:
		parsePairOfArguments(argv[1], argv[2], validArgs, paths, pathHasBeenGiven);
	case 1:
		break;
	default:
		validArgs = false;
		break;
	}
	return validArgs;
}

void parsePairOfArguments(char * type, char * path, bool & validArgs, vector<string>& paths, vector<bool>& pathHasBeenGiven) {

	if (strcmp(type, "-maze_path") == 0 && pathExist(path)) { // .maze folder path
		if (!pathHasBeenGiven[MAZEPATH_INDEX]) {
			pathHasBeenGiven[MAZEPATH_INDEX] = true;
			paths[MAZEPATH_INDEX] = path;
		}
		else validArgs = false;
	}
	else if (strcmp(type, "-algorithm_path") == 0 && pathExist(path)) { // .so folder path
		if (!pathHasBeenGiven[ALGOPATH_INDEX]) {
			pathHasBeenGiven[ALGOPATH_INDEX] = true;
			paths[ALGOPATH_INDEX] = path;
		}
		else validArgs = false;
	}
	else if (strcmp(type, "-output") == 0 && pathExist(path)) { // .output folder path
		if (!pathHasBeenGiven[OUTPUTPATH_INDEX]) {
			pathHasBeenGiven[OUTPUTPATH_INDEX] = true;
			paths[OUTPUTPATH_INDEX] = path;
		}
		else validArgs = false;
	}
	else validArgs = false;
}





