#ifndef MATCHMANAGER_H
#define MATCHMANAGER_H

#include "AlgorithmRegistration.h"
#include "GameManager.h"
#include <stack>
#include <cassert>
using namespace std;

/* ------------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------------- */
/* -------------------------------------- MatchManager class --------------------------------------- */
/* ------------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------------- */

class MatchManager {
private:

	/* ---------------------------------------------------------------------------------------- */
	/* --------------------------------- MatchManager members --------------------------------- */
	/* ---------------------------------------------------------------------------------------- */

	// These members contain all the details about the maze.
	string 								m_name;
	int 								m_maxSteps, m_rowsNum, m_colsNum;
	MazeBoard 							m_board;
	Coordinate 							m_playerLocation;
	Coordinate							m_endLocation;
	Coordinate							m_bookmarkVector;

	// These members contain details about the maze solver algoritms and GameManager objects.
	stack<AlgorithmPair>				m_AlgorithmStack;
	vector<string>&						m_algorithmNameVector;
	MoveListMap							m_moveListMap; // key is algorithm name, value is its moveList
	const int							m_numOfThreads;

	// this is a mutex object used for 
	mutex								m_mtx;

public:

	/* ---------------------------------------------------------------------------------------- */
	/* ----------------------------- MatchManager public functions ---------------------------- */
	/* ---------------------------------------------------------------------------------------- */

	/* ------------------------------------- c'tor -------------------------------------- */

	MatchManager(string name, int maxSteps, int rowsNum, int colsNum, MazeBoard board,
		Coordinate playerLocation, Coordinate endLocation, vector<string> & algoNameVec, const int numOfThreads) :
		m_name(name), m_maxSteps(maxSteps), m_rowsNum(rowsNum),
		m_colsNum(colsNum), m_board(board), m_playerLocation(playerLocation), m_endLocation(endLocation),
		m_bookmarkVector(playerLocation), m_algorithmNameVector(algoNameVec), m_numOfThreads(numOfThreads) {};

	/* -------------------------- GameManager main functions ---------------------------- */

	void								createGameManagers		();
	void								threadFunc				();

	/* ---------------------------- Other helper functions ------------------------------ */

	inline string &						getName					() { return m_name; }
	inline MoveListMap					getMoveListMap			() { return m_moveListMap; }
	AlgorithmPair						getAlgorithmFromStack	();
	void								fillStack				();
	
};





/* ------------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------------- */
/* ----------------------------------- AlgorithmRegistrar class ------------------------------------ */
/* ------------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------------- */

class AlgorithmRegistrar {
private:

	/* ---------------------------------------------------------------------------------------- */
	/* --------------------------- AlgorithmRegistrar members --------------------------------- */
	/* ---------------------------------------------------------------------------------------- */

	// All of the algorithms will be registered in the static member <instance>'s field <algorithmFactoryVec>.
	static AlgorithmRegistrar			instance;
	vector<AlgorithmFactory>			algorithmFactoryVec;

	/* ---------------------------------------------------------------------------------------- */
	/* ------------------------- AlgorithmRegistrar private functions ------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	
	inline void							registerAlgorithm (AlgorithmFactory algorithmFactory)
										{ instance.algorithmFactoryVec.push_back(algorithmFactory); }
public:

	/* ---------------------------------------------------------------------------------------- */
	/* -------------------------- AlgorithmRegistrar public functions ------------------------- */
	/* ---------------------------------------------------------------------------------------- */

	friend class						AlgorithmRegistration;
	inline vector<AlgorithmFactory>&	getAlgoFactoryVec	() { return algorithmFactoryVec; }
	inline static AlgorithmRegistrar&	getInstance			() { return instance; }
	inline void							clearVector			() { algorithmFactoryVec.clear(); }
};
#endif