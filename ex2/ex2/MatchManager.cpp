#include "MatchManager.h"
#include "AlgorithmRegistration.h"


//define the static variable
MatchManager MatchManager::matchManagerObj;

void MatchManager::createGameManagers()
{
	// let's assume we have an AbstractAlgorithm vector:
	vector<unique_ptr<AbstractAlgorithm>> algorithmVec;
	for (auto& factoryMethod : factoryVector) {
		m_gameManagerVector.push_back(GameManager(m_name, m_maxSteps, m_rowsNum, m_colsNum,
			m_board, m_playerLocation, m_endLocation, factoryMethod()));
	}
}

void MatchManager::activateGameManagers()
{
	for (int i = 0; i < m_gameManagerVector.size(); i++) {
		m_moveListVector.push_back(m_gameManagerVector[i].play());
	}
}

void MatchManager::registerAlgorithm(function<unique_ptr<AbstractAlgorithm>()> factoryMethod)
{
	factoryVector.push_back(factoryMethod);
}

MatchManager & MatchManager::getMatchManager()
{
	return matchManagerObj;
}
