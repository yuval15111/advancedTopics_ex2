#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "MatchManager.h"
#include <regex>
#include <dlfcn.h>

#define BUF_SIZE 1024

class FileHandler {
private:

	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------- File Handler members -------------------------------- */
	/* ---------------------------------------------------------------------------------------- */

	// maze, algorithm, output paths and num of threads
	const string &			m_mazePath;
	const string &			m_algorithmPath;
	const string &			m_outputPath;
	const int				m_numOfThreads;

	// match, algorithm and dl vectors
	vector<unique_ptr<MatchManager>> m_matchVector;
	vector<string>			m_algorithmNameVector;
	vector<void *>			m_dlVector;

	// parsing errors struct
	Errors					m_errors;

	/* ---------------------------------------------------------------------------------------- */
	/* ----------------------------- File Handler private functions --------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	
	/* -------------- maze and so files searching and reading functions ----------------- */
	FILE *					execCmd							(const char * cmd);
	void					generateAlgorithmsFromSoFiles	(FILE * dl);
	void					generateMatchesFromMazeFiles	(FILE * dl);

	/* ------------------------------ maze parsing functions ---------------------------- */
	unique_ptr<MatchManager>parseMaze						(ifstream& fin);
	string					getName							(ifstream& fin, string & line);
	int						getIntValue						(ifstream& fin, const string & input,
															 const ErrorType error, string & line);
	MazeBoard				getBoard						(ifstream& fin, const int rows, const int cols,
															 Coordinate & playerLocation, Coordinate & endLocation,
															 string & line);
	void					handleSpecialChar				(const char c, Coordinate & location,
															 const int i, const int j, bool & seenChar,
															 string & line, const ErrorType e);
	void					handleInvalidChar				(const char c, const int i, const int j);
	void					checkErrors						(void* (titleFunc));
	inline void				pushError						(ErrorType type, const string & str)
																{ m_errors.list.push_back(ErrorPair(type, str)); }

	/* ----------------------------- other helper functions ----------------------------- */
	vector<string>					getMazeNamesVector		();
	vector<map<string, MoveList>>getAllMatchesMoveListMaps();

	/* ---------------------------------------------------------------------------------------- */
	/* ----------------------------- File Handler public functions ---------------------------- */
	/* ---------------------------------------------------------------------------------------- */

public:
	/* ---------------------------------- c'tor and d'tor ------------------------------- */
	FileHandler(vector<string>& arguments)	:	m_mazePath(arguments[MAZEPATH_INDEX]),
											m_algorithmPath(arguments[ALGOPATH_INDEX]),
											m_outputPath(arguments[OUTPUTPATH_INDEX]),
											m_numOfThreads(stoi(arguments[NUMTHREADS_INDEX])) {};
	~FileHandler											();

	/* - functions responsible for algorithms and matches generation & output creation -- */
	void					getAlgorithms					();
	void					getMatchesAndPlay				();
	void					createOutput					();
};

#endif