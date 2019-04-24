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
	bool					outputPathExists = false;
	bool					algorithmPathExists = false;
	//vector<ifstream>		m_mazeFiles;
	ifstream				m_mazeFile;
	vector<void*>			m_algorithmDL;
	//TODO: insert algorithm .so file field
	ofstream				m_outputFile;
	Errors					m_errors;
	GameManager *			m_manager = nullptr;
	bool					createDLVector(const string& path);
	bool					parsePairOfArguments(char * type, char * path);
	inline void				allowParsing(bool allow) { m_errors.allowParsing = allow; }
	void					checkErrors(void* (titleFunc));
	inline void				pushError(ErrorType type, const string & str) { m_errors.list.push_back(Pair(type, str)); }
	string					getName(string & line);
	int						getIntValue(const string & input, const ErrorType error, string & line);
	MazeBoard				getBoard(const int rows, const int cols, Coordinate & playerLocation, Coordinate & endLocation, string & line);
	void					handleSpecialChar(const char c, Coordinate & location, const int i, const int j, bool & seenChar, string & line, const ErrorType e);
	void					handleInvalidChar(const char c, const int i, const int j);

public:
	FileHandler(int argc, char* argv[]);
	~FileHandler();
	inline GameManager *	getManager() { return m_manager; }
	void					parseInput();
	inline bool				parsingIsAllowed() { return m_errors.allowParsing; }
	void					pushActionsToOutputFile(vector<char> actions);
};

#endif