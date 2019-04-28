#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "GameManager.h"
#include <regex>
#include <stdio.h>
//#include <dlfcn.h>

#define BUF_SIZE 1024

class FileHandler {
private:
	bool					mazePathExists = false;
	bool					algorithmPathExists = false;
	bool					outputPathExists = false;
	vector<MazePair>		m_mazeVector;
	vector<AlgorithmPair>	m_algorithmVector;
	vector<OutputPair>		m_outputVector;
	string					m_outputPath;
	Errors					m_errors;
	GameManager *			m_manager = nullptr;
	void					initVectorsByCurrDirectory(const string& path);
	void					createDLVector(const string& path);
	void					createOutputVector();
	void					parsePairOfArguments(char * type, char * path);
	void					checkErrors(void* (titleFunc));
	inline void				pushError(ErrorType type, const string & str) { m_errors.list.push_back(ErrorPair(type, str)); }
	string					getName(string & line);
	int						getIntValue(const string & input, const ErrorType error, string & line);
	MazeBoard				getBoard(const int rows, const int cols, Coordinate & playerLocation, Coordinate & endLocation, string & line);
	void					handleSpecialChar(const char c, Coordinate & location, const int i, const int j, bool & seenChar, string & line, const ErrorType e);
	void					handleInvalidChar(const char c, const int i, const int j);

public:
	bool					wrongArgumentsFormat = false;
	FileHandler(int argc, char* argv[]);
	~FileHandler();
	inline GameManager *	getManager() { return m_manager; }
	void					parseInput();
	void					pushActionsToOutputFile(vector<char> actions);
};

#endif