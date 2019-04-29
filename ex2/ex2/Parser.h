#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "GameManager.h"
#include <regex>
#include <stdio.h>
//#include <dlfcn.h>

#define BUF_SIZE 1024

class Parser {
private:
	bool					mazePathExists = false;
	bool					algorithmPathExists = false;
	bool					outputPathExists = false;
	bool					invalidArguments = false;

	list<MazePair>			m_mazeList;
	list<AlgorithmPair>		m_algorithmList;
	list<ofstream>			m_outputList;
	string					m_outputPath;

	void					initVectorsByCurrDirectory(const string& path);
	void					createMazeList(const string& path);
	void					createAlgorithmList(const string& path);
	void					createOutputList();
	void					parsePairOfArguments(char * type, char * path);

	Errors					m_errors;
	GameManager *			m_manager = nullptr;

	void					checkErrors(void* (titleFunc));
	inline void				pushError(ErrorType type, const string & str) { m_errors.list.push_back(ErrorPair(type, str)); }
	string					getName(string & line);
	int						getIntValue(const string & input, const ErrorType error, string & line);
	MazeBoard				getBoard(const int rows, const int cols, Coordinate & playerLocation, Coordinate & endLocation, string & line);
	void					handleSpecialChar(const char c, Coordinate & location, const int i, const int j, bool & seenChar, string & line, const ErrorType e);
	void					handleInvalidChar(const char c, const int i, const int j);

public:
	Parser(int argc, char* argv[]);
	~Parser();
	inline GameManager *	getManager() { return m_manager; }
	void					parseInput();
	void					pushActionsToOutputFile(vector<char> actions);
	inline bool				invalidArgs() { return invalidArguments; }
};

#endif