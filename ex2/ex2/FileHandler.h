#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "MatchManager.h"
#include <regex>
#include <dlfcn.h>

#define BUF_SIZE 1024
#define COLUMN_LENGTH 30

class FileHandler {
private:
	string						m_mazePath = ".";
	string						m_algorithmPath = ".";
	string						m_outputPath = ".";

	vector<MatchManager *>		m_matchVector;
	vector<string>				m_algorithmNameVector;
	vector<vector<char>>		m_outputVector;

	vector<void *>				dlVector;
	Errors						m_errors;

	void						checkErrors(void* (titleFunc));
	inline void					pushError(ErrorType type, const string & str) { m_errors.list.push_back(ErrorPair(type, str)); }
	
	FILE *						execCmd(const char * cmd);
	ifstream *					openIFstream(const char * filename);
	void						iterateOverMazeFiles(FILE * dl);
	void						iterateOverSOFiles(FILE * dl);

	void						printSeperationRow(unsigned int num_of_mazes);
	void						printTitles(unsigned int num_of_mazes);
	void						printAlgorithmName(string & algoName);
	void						printAlgorithmResultOnAllMazes(unsigned int i, string & algoName);
	void						createOutputFile(string & algoName, string & mazeName, vector<char> & moveList);
	string						getAvaliableFileName(string & algoName, string & mazeName);

	MatchManager *				parseMaze(ifstream * fin);
	string						getName(ifstream * fin, string & line);
	int							getIntValue(ifstream * fin, const string & input, const ErrorType error, string & line);
	MazeBoard					getBoard(ifstream * fin, const int rows, const int cols, Coordinate & playerLocation, Coordinate & endLocation, string & line);
	void						handleSpecialChar(const char c, Coordinate & location, const int i, const int j, bool & seenChar, string & line, const ErrorType e);
	void						handleInvalidChar(const char c, const int i, const int j);

public:
	FileHandler(vector<string>& paths);
	~FileHandler();
	void						getMatches();
	void						getAlgorithms();
	void						createOutput();
	void						pushActionsToOutputFile(ofstream & fout, vector<char> actions);
	inline bool					outputPathExists() { return m_outputPath.compare("") != 0; }
};

#endif