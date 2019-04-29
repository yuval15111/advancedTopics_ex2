#pragma once

#include "AlgorithmRegistration.h"
#include "Parser.h"
#include <vector>
#include <functional>
using namespace std;

class MatchManager {
	static MatchManager matchManagerObj;
	vector <function<unique_ptr<AbstractAlgorithm>()>> factoryVector;

private:
	string 					m_name;
	int 					m_maxSteps, m_rowsNum, m_colsNum;
	MazeBoard 				m_board;
	Coordinate 				m_playerLocation;
	Coordinate				m_endLocation;
	Coordinate				m_bookmarkVector;
	vector<AlgorithmPair>&	m_algorithmVector;

public:
	MatchManager(string name, int maxSteps, int rowsNum, int colsNum,
		MazeBoard board, Coordinate playerLocation, Coordinate endLocation, vector<AlgorithmPair> & algoVec) :
		m_name(name), m_maxSteps(maxSteps), m_rowsNum(rowsNum),
		m_colsNum(colsNum), m_board(board), m_playerLocation(playerLocation),
		m_endLocation(endLocation), m_bookmarkVector(playerLocation), m_algorithmVector(algoVec) {};

	void registerAlgorithm(function<unique_ptr<AbstractAlgorithm>()> factoryMethod);
	static MatchManager& getMatchManager();
};