#ifndef MAINAUX_H
#define MAINAUX_H

#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <list>
#include <utility>
#include <fstream>
#include <sys/stat.h>
#include <memory>
#include <stdio.h>

#include "AbstractAlgorithm.h"

#define MAX_INT 2147483647
#define ABS(x,y) x > y ? x - y : y - x

#define MAXSTEPS "MaxSteps"
#define ROWS "Rows"
#define COLS "Cols"
#define SPACE_CHAR ' '
#define PLAYER_CHAR '@'
#define END_CHAR '$'
#define WALL_CHAR '#'

#define LEGALSTEP 0
#define HITWALL 1
#define HITBOOKMARK 2
#define NUM_OF_STEPS 4

#define MAZEPATH_INDEX 0
#define ALGOPATH_INDEX 1
#define OUTPUTPATH_INDEX 2

enum class ErrorType {
	MaxStepsError, RowsError, ColsError,
	MissingPlayerChar, MissingEndChar,
	MoreThanOnePlayerChar, MoreThanOneEndChar,
	WrongChar
};

using namespace std;
using MazeRow = vector<char>;
using MazeBoard = vector<MazeRow>;
using Coordinate = pair<int, int>;
using MazePair = pair<string, ifstream*>; // the name of the maze and the stream
using AlgorithmPair = pair<string, void*>; // the name of the algorithm and the .so file
using OutputPair = pair<string, ofstream&>; // the name of the output file and the stream
using ErrorPair = pair<ErrorType, string>;
using ErrorList = vector<ErrorPair>;
typedef void(*Func) (const string & str);
typedef void(*FuncNoArgs) ();
typedef AbstractAlgorithm::Move Move;

Move operator!(const Move& a);

// Event messages
void printWinMessage(const int numOfSteps);
void printLostMessage(const int numOfSteps);
void printBadAlgorithmWarning(string algoName);
void printBadMazeWarning(string mazeName);
void printWrongArgumentsFormatError();
void printHeaderErrorTitle();
void printMazeErrorTitle();
void printMaxStepsError(const string & str);
void printRowsError(const string & str);
void printColsError(const string & str);
void printMissingPlayerCharError(const string & str);
void printMissingEndCharError(const string & str);
void printMoreThanOnePlayerCharError(const string & str);
void printMoreThanOneEndCharError(const string & str);
void printWrongCharError(const string & str);

struct Errors {
	map<ErrorType, Func> fmap = {
		{ErrorType::MaxStepsError, &printMaxStepsError},
		{ErrorType::RowsError, &printRowsError},
		{ErrorType::ColsError, &printColsError},
		{ErrorType::MissingPlayerChar, &printMissingPlayerCharError},
		{ErrorType::MissingEndChar, &printMissingEndCharError},
		{ErrorType::MoreThanOnePlayerChar, &printMoreThanOnePlayerCharError},
		{ErrorType::MoreThanOneEndChar, &printMoreThanOneEndCharError},
		{ErrorType::WrongChar, &printWrongCharError}
	};
	bool no_parsing_Errors = true;
	vector<ErrorPair> list;
};

bool fileExists(const char* path);
char getMoveChar(const Move& a);
void updateCoordinate(Coordinate & c, const int i, const int j);
bool endsWith(const string & mainStr, const string & toMatch);
bool initPaths(int argc, char * argv[], string paths[3], bool pathExists[3]);
void parsePairOfArguments(char * type, char * path, bool & validArgs, string paths[3], bool pathExists[3]);

#endif