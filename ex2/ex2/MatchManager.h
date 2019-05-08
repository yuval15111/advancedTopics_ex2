#ifndef MATCHMANAGER_H
#define MATCHMANAGER_H

#include "AlgorithmRegistration.h"
#include "GameManager.h"
#include <vector>
#include <functional>
#include <cassert>
using namespace std;

class MatchManager {
	/*static MatchManager matchManagerObj;
	vector <function<unique_ptr<AbstractAlgorithm>()>> factoryVector;*/

	//map <string, function<unique_ptr<AbstractAlgorithm>()>> factoryVector;

private:
	string 					m_name;
	int 					m_maxSteps, m_rowsNum, m_colsNum;
	MazeBoard 				m_board;
	Coordinate 				m_playerLocation;
	Coordinate				m_endLocation;
	Coordinate				m_bookmarkVector;
	//vector<AlgorithmPair>&	m_algorithmVector;
	vector<string>&			m_algorithmNameVector;
	vector<GameManager>		m_gameManagerVector;
	vector<vector<char>>	m_moveListVector;
	//vector<unique_ptr<AbstractAlgorithm>> v;

public:
	MatchManager(string name, int maxSteps, int rowsNum, int colsNum,
		MazeBoard board, Coordinate playerLocation, Coordinate endLocation, vector<string> & algoNameVec) :
		m_name(name), m_maxSteps(maxSteps), m_rowsNum(rowsNum),
		m_colsNum(colsNum), m_board(board), m_playerLocation(playerLocation),
		m_endLocation(endLocation), m_bookmarkVector(playerLocation), m_algorithmNameVector(algoNameVec) {};
	void createGameManagers();
	void activateGameManagers();
	//void registerAlgorithm(function<unique_ptr<AbstractAlgorithm>()> factoryMethod);
	inline int algorithmsCount() { return m_algorithmNameVector.size(); }
	//static MatchManager& getMatchManager();
};

class AlgorithmRegistrar {
	vector<function<unique_ptr<AbstractAlgorithm>()>> algorithmFactoryVec;
	inline void registerAlgorithm(std::function<unique_ptr<AbstractAlgorithm>()> algorithmFactory) {
		instance.algorithmFactoryVec.push_back(algorithmFactory);
	}
public:
	friend class AlgorithmRegistration;
	vector<unique_ptr<AbstractAlgorithm>> getAlgorithms()const {
		vector<unique_ptr<AbstractAlgorithm>> algorithms;
		for (auto algorithmFactoryFunc : algorithmFactoryVec) {
			algorithms.push_back(move(algorithmFactoryFunc()));
		}
		return algorithms;
	}
	vector<function<unique_ptr<AbstractAlgorithm>()>>& getAlgoFactoryVec() {
		return algorithmFactoryVec;
	}
	static AlgorithmRegistrar& getInstance() {
		return instance;
	}
private:
	static AlgorithmRegistrar instance;
};
#endif