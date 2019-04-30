#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "GameManager.h"
#include "MatchManager.h"
#include <regex>
#include <stdio.h>
#include <dlfcn.h>

#define BUF_SIZE 1024

class Parser {
private:
	bool						mazePathExists = false;
	bool						algorithmPathExists = false;
	bool						outputPathExists = false;
	bool						invalidArguments = false;

	vector<MazePair>			m_mazeVector;
	vector<AlgorithmPair>		m_algorithmVector;
	vector<ofstream>			m_outputVector;
	string						m_outputPath;
	
	void						initVectorsByCurrDirectory(const string& path);
	void						createMazeVector(const string& path);
	void						createAlgorithmVector(const string& path);
	void						createOutputVector();
	void						parsePairOfArguments(char * type, char * path);

	Errors						m_errors;
	GameManager *				m_manager = nullptr;

	void						checkErrors(void* (titleFunc));
	inline void					pushError(ErrorType type, const string & str) { m_errors.list.push_back(ErrorPair(type, str)); }
	string						getName(ifstream * fin, string & line);
	int							getIntValue(ifstream * fin, const string & input, const ErrorType error, string & line);
	MazeBoard					getBoard(ifstream * fin, const int rows, const int cols, Coordinate & playerLocation, Coordinate & endLocation, string & line);
	void						handleSpecialChar(const char c, Coordinate & location, const int i, const int j, bool & seenChar, string & line, const ErrorType e);
	void						handleInvalidChar(const char c, const int i, const int j);

public:
	Parser(int argc, char* argv[]);
	~Parser();
	MatchManager *				parseInput(ifstream * fin);
	void						pushActionsToOutputFile(ofstream & fout, vector<char> actions);
	inline bool					invalidArgs() { return invalidArguments; }
	inline vector<MazePair>&	getMazeVector() { return m_mazeVector; }
};

#endif