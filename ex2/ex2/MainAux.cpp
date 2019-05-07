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

void pushLogsToOutputFiles(vector<MatchManager*> matchVector, bool outputPathExists)
{
	/*for (int i = 0; i < matchVector.size(); i++) {
		for (int j = 0; j < matchVector[i]->algorithmsCount(); j++) {

		}
		matchVector[i]->
	}*/
}
