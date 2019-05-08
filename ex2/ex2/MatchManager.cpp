#include "MatchManager.h"
#include "AlgorithmRegistration.h"


//define the static variable
//MatchManager MatchManager::matchManagerObj;

void MatchManager::createGameManagers()
{
	AlgorithmRegistrar& registrar = AlgorithmRegistrar::getInstance();
	auto algorithms = registrar.getAlgorithms();
	for (auto& algorithm : algorithms) {
		m_gameManagerVector.push_back(GameManager(m_name, m_maxSteps, m_rowsNum, m_colsNum,
			m_board, m_playerLocation, m_endLocation, move(algorithm)));
	}
}

void MatchManager::activateGameManagers()
{
	for (unsigned int i = 0; i < m_gameManagerVector.size(); i++) {
		m_moveListVector.push_back(m_gameManagerVector[i].play());
	}
}

//void MatchManager::registerAlgorithm(function<unique_ptr<AbstractAlgorithm>()> factoryMethod)
//{
//	factoryVector.push_back(factoryMethod);
//}

//MatchManager & MatchManager::getMatchManager()
//{
//	return matchManagerObj;
//}
