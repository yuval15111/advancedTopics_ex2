#ifndef MAINAUX_H
#define MAINAUX_H

#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <fstream>
#include <sys/stat.h>

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

enum class ErrorType {
	MissingInput, MissingOutput, BadInputAddress, BadOutputAddress,
	MaxStepsError, RowsError, ColsError,
	MissingPlayerChar, MissingEndChar,
	MoreThanOnePlayerChar, MoreThanOneEndChar,
	WrongChar
};

using namespace std;
using MazeRow = vector<char>;
using MazeBoard = vector<MazeRow>;
using Coordinate = pair<int, int>;
using Pair = pair<ErrorType, string>;
using ErrorList = vector<Pair>;
typedef void(*Func) (const string & str);
typedef void(*FuncNoArgs) ();
typedef AbstractAlgorithm::Move Move;

Move operator!(const Move& a);

// Event messages
void printWinMessage(const int numOfSteps);
void printLostMessage(const int numOfSteps);
void printMissingInputError(const string & str);
void printMissingOutputError(const string & str);
void printBadInputAddressError(const string & str);
void printBadOutputAddressError(const string & str);
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
		{ErrorType::MissingInput, &printMissingInputError},
		{ErrorType::MissingOutput, &printMissingOutputError},
		{ErrorType::BadInputAddress, &printBadInputAddressError},
		{ErrorType::BadOutputAddress, &printBadOutputAddressError},
		{ErrorType::MaxStepsError, &printMaxStepsError},
		{ErrorType::RowsError, &printRowsError},
		{ErrorType::ColsError, &printColsError},
		{ErrorType::MissingPlayerChar, &printMissingPlayerCharError},
		{ErrorType::MissingEndChar, &printMissingEndCharError},
		{ErrorType::MoreThanOnePlayerChar, &printMoreThanOnePlayerCharError},
		{ErrorType::MoreThanOneEndChar, &printMoreThanOneEndCharError},
		{ErrorType::WrongChar, &printWrongCharError}
	};
	vector<pair<ErrorType, string>> list;
	bool no_IO_Errors = true;
	bool no_parsing_Errors = true;
	bool allowParsing = false;
};

bool fileExists(const char* path);
char getActionChar(const Move& a);
void updateCoordinate(Coordinate & c, const int i, const int j);

#endif