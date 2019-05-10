#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "MatchManager.h"
#include <regex>
#include <stdio.h>
#include <dlfcn.h>

#define BUF_SIZE 1024

class FileHandler {
private:
	bool						m_mazePathExists = false;
	bool						m_algorithmPathExists = false;
	bool						m_outputPathExists = false;
	bool						m_invalidArguments = false;

	vector<MatchManager *>		m_matchVector;
	vector<string>				m_algorithmNameVector;
	//vector<ofstream>			m_outputVector;
	vector<vector<char>>		m_outputVector;
	string						m_algorithmPath = ".";
	string						m_mazePath = ".";
	string						m_outputPath = ".";
	vector<void *>				dlVector;
	
	void						initVectors();
	void						createMatchVector();
	void						createAlgorithmVector();
	void						createOutput();
	void						parsePairOfArguments(char * type, char * path);

	Errors						m_errors;

	void						checkErrors(void* (titleFunc));
	inline void					pushError(ErrorType type, const string & str) { m_errors.list.push_back(ErrorPair(type, str)); }
	string						getName(ifstream * fin, string & line);
	int							getIntValue(ifstream * fin, const string & input, const ErrorType error, string & line);
	MazeBoard					getBoard(ifstream * fin, const int rows, const int cols, Coordinate & playerLocation, Coordinate & endLocation, string & line);
	void						handleSpecialChar(const char c, Coordinate & location, const int i, const int j, bool & seenChar, string & line, const ErrorType e);
	void						handleInvalidChar(const char c, const int i, const int j);

public:
	FileHandler(int argc, char* argv[]);
	~FileHandler();
	MatchManager *				parseInput(ifstream * fin);
	void						pushActionsToOutputFile(ofstream & fout, vector<char> actions);
	inline bool					invalidArgs() { return m_invalidArguments; }
	inline bool					outputPathExists() { return m_outputPathExists; }
};

#endif