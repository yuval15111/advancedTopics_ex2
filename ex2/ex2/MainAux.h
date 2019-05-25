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
#include <unistd.h>
#include <sys/types.h>
#include <memory>
#include <stdio.h>
#include <functional>
#include <thread>
#include <mutex>

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
#define NUMTHREADS_INDEX 3

#define TABLE_COLUMN_LENGTH 30

/* ------------------------------ ErrorType enum declaration  ----------------------------- */

enum class ErrorType {
	MaxStepsError, RowsError, ColsError,
	MissingPlayerChar, MissingEndChar,
	MoreThanOnePlayerChar, MoreThanOneEndChar,
	WrongChar
};

/* ---------------------------- typedefs/using for readability ---------------------------- */

using namespace std;
using MazeRow = vector<char>;
using MazeBoard = vector<MazeRow>;
using Coordinate = pair<int, int>;
using ErrorPair = pair<ErrorType, string>;
using ErrorList = vector<ErrorPair>;
using AlgorithmFactory = function<unique_ptr<AbstractAlgorithm>()>;
using MoveList = vector<char>;
using MoveListMap = map<string, MoveList>;
using AlgorithmPair = pair<string, AlgorithmFactory>;
typedef void(*Func) (const string & str);
typedef void(*FuncNoArgs) ();
typedef AbstractAlgorithm::Move Move;

/* ------------------------------- Event messages functions ------------------------------- */

void		printWinMessage					(const int numOfSteps);
void		printLostMessage				(const int numOfSteps);
void		printBadAlgorithmWarning		(const string & algoName);
void		printBadMazeWarning				(const string & mazeName);
void		printStreamError				(const string & filename);
void		printPopenError					();
void		printWrongArgumentsFormatError	();
void		printHeaderErrorTitle			();
void		printMazeErrorTitle				();
void		printMaxStepsError				(const string & str);
void		printRowsError					(const string & str);
void		printColsError					(const string & str);
void		printMissingPlayerCharError		(const string & str);
void		printMissingEndCharError		(const string & str);
void		printMoreThanOnePlayerCharError	(const string & str);
void		printMoreThanOneEndCharError	(const string & str);
void		printWrongCharError				(const string & str);

/* ------------------------------ Error struct declaration  ------------------------------ */

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

/* ------------------------------- Parsing helper functions ------------------------------ */

bool		fileExists						(const char* path);
bool		pathExist						(const char* path);
bool		endsWith						(const string & mainStr, const string & toMatch);
bool		initArgumentVector				(int argc, char * argv[], vector<string>& argVec);
void		parsePairOfArguments			(char * type, char * path, bool & validArgs, vector<string>& argVec,
											 vector<bool>& argHasBeenGiven);

/* -------------------------- GameManager play() helper functions ------------------------ */

char		getMoveChar						(const Move& a);
Move		operator!						(const Move& a);
void		updateCoordinate				(Coordinate & c, const int i, const int j);


/* --------------------- FileHandler createOutput() helper functions --------------------- */
void		printSeperationRow				(const unsigned int num_of_mazes);
void		printTitles						(const unsigned int num_of_mazes, const vector<string> & mazeNameVector);
void		printAlgorithmName				(const string & algoName);
void		printAlgorithmResultOnAllMazes	(const string & path, const unsigned int num_of_mazes,
											 const string & algoName, const vector<MoveListMap> & allMatchesMoveLists,
											 const vector<string> & mazeNameVector);
inline bool	outputPathExists				(const string & path) { return path.compare("") != 0; }
void		createOutputFile				(const string & path, const string & algoName, const string & mazeName,
											 const MoveList & moveList);
string		getAvaliableFileName			(const string & path, const string & algoName, const string & mazeName);
void		pushMovesToOutputFile			(ofstream & fout, const MoveList& moveList);


#endif