#ifndef MATCHMANAGER_H
#define MATCHMANAGER_H

#include "AlgorithmRegistration.h"
#include "GameManager.h"
#include <vector>
#include <functional>
#include <cassert>
using namespace std;

class MatchManager {
private:
	string 					m_name;
	int 					m_maxSteps, m_rowsNum, m_colsNum;
	MazeBoard 				m_board;
	Coordinate 				m_playerLocation;
	Coordinate				m_endLocation;
	Coordinate				m_bookmarkVector;
	vector<string>&			m_algorithmNameVector;
	vector<GameManager>		m_gameManagerVector;
	vector<vector<char>>	m_moveListVector;

public:
	MatchManager(string name, int maxSteps, int rowsNum, int colsNum,
		MazeBoard board, Coordinate playerLocation, Coordinate endLocation, vector<string> & algoNameVec) :
		m_name(name), m_maxSteps(maxSteps), m_rowsNum(rowsNum),
		m_colsNum(colsNum), m_board(board), m_playerLocation(playerLocation),
		m_endLocation(endLocation), m_bookmarkVector(playerLocation), m_algorithmNameVector(algoNameVec) {};
	void createGameManagers();
	void activateGameManagers();
	inline string & getName() { return m_name; }
	inline int algorithmsCount() { return m_algorithmNameVector.size(); }
	inline vector<vector<char>> getMoveListVector() { return m_moveListVector; }
};

class AlgorithmRegistrar {
private:
	static AlgorithmRegistrar instance;
	vector<function<unique_ptr<AbstractAlgorithm>()>> algorithmFactoryVec;
	inline void registerAlgorithm(std::function<unique_ptr<AbstractAlgorithm>()> algorithmFactory) {

		cout << "MM - registare algo "  << endl;
		instance.algorithmFactoryVec.push_back(algorithmFactory);
	}
public:
	friend class AlgorithmRegistration;
	vector<function<unique_ptr<AbstractAlgorithm>()>>& getAlgoFactoryVec() {
		return algorithmFactoryVec;
	}
	static AlgorithmRegistrar& getInstance() {
		return instance;
	}
};
#endif