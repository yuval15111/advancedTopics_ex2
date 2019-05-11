#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "MatchManager.h"
#include <regex>
#include <dlfcn.h>

#define BUF_SIZE 1024
#define COLUMN_LENGTH 30

class FileHandler {
private:
	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------- File Handler members -------------------------------- */
	/* ---------------------------------------------------------------------------------------- */

	// maze, algorithm and output paths
	const string &				m_mazePath;
	const string &				m_algorithmPath;
	const string &				m_outputPath;

	// match, algorithm and dl vectors
	vector<MatchManager *>	m_matchVector;
	vector<string>			m_algorithmNameVector;
	vector<void *>			m_dlVector;

	// parsing errors struct
	Errors					m_errors;

	/* ---------------------------------------------------------------------------------------- */
	/* ----------------------------- File Handler private functions --------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	
	/* ----------------- maze and so files searching and reading functions -------------------- */
	FILE *					execCmd(const char * cmd);
	ifstream *				openIFstream(const char * filename);
	void					generateMatchesFromMazeFiles(FILE * dl);
	void					generateAlgorithmsFromSoFiles(FILE * dl);

	/* --------------------------- output creation helper functions --------------------------- */
	void					printSeperationRow(unsigned int num_of_mazes);
	void					printTitles(unsigned int num_of_mazes);
	void					printAlgorithmName(string & algoName);
	void					printAlgorithmResultOnAllMazes(unsigned int num_of_mazes, unsigned int i, string & algoName);
	inline bool				outputPathExists() { return m_outputPath.compare("") != 0; }
	void					createOutputFile(string & algoName, string & mazeName, vector<char> & moveList);
	string					getAvaliableFileName(string & algoName, string & mazeName);
	void					pushMovesToOutputFile(ofstream & fout, vector<char>& moveList);

	/* --------------------------------- maze parsing functions ------------------------------- */
	MatchManager *			parseMaze(ifstream * fin);
	string					getName(ifstream * fin, string & line);
	int						getIntValue(ifstream * fin, const string & input, const ErrorType error, string & line);
	MazeBoard				getBoard(ifstream * fin, const int rows, const int cols, Coordinate & playerLocation, Coordinate & endLocation, string & line);
	void					handleSpecialChar(const char c, Coordinate & location, const int i, const int j, bool & seenChar, string & line, const ErrorType e);
	void					handleInvalidChar(const char c, const int i, const int j);
	void					checkErrors(void* (titleFunc));
	inline void				pushError(ErrorType type, const string & str) { m_errors.list.push_back(ErrorPair(type, str)); }



	/* ---------------------------------------------------------------------------------------- */
	/* ----------------------------- File Handler public functions ---------------------------- */
	/* ---------------------------------------------------------------------------------------- */

public:
	/* ------------------------------------- c'tor and d'tor ---------------------------------- */
	FileHandler(vector<string>& paths)	:	m_mazePath(paths[MAZEPATH_INDEX]),
											m_algorithmPath(paths[ALGOPATH_INDEX]),
											m_outputPath(paths[OUTPUTPATH_INDEX]) {};
	~FileHandler();

	/* ---- functions responsible for algorithms and matches generation and output creation --- */
	void					getAlgorithms();
	void					getMatchesAndPlay();
	void					createOutput();
};

#endif