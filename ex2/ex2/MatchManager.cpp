#include "MatchManager.h"
#include "AlgorithmRegistration.h"

void MatchManager::createGameManagers()
{
	AlgorithmRegistrar registrar = AlgorithmRegistrar::getInstance();
	auto algorithms = registrar.getAlgoFactoryVec();
	for (auto& algorithm : algorithms) {
		 m_moveListVector.push_back(GameManager(m_name, m_maxSteps, m_rowsNum, m_colsNum,
			m_board, m_playerLocation, m_endLocation, move(algorithm())).play());
	}
}

AlgorithmRegistrar AlgorithmRegistrar::instance;