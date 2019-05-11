#include "MatchManager.h"
#include "AlgorithmRegistration.h"

/* -------------------------- GameManager main functions ---------------------------- */

/*	This function creates a GameManager obj for each factoryMethod in the registrar,
	and runs all games using their algorithms instantly using the GameManager play() function.
	Afterwards, the returned value (a MoveList object) is pushed to the instance MoveListVector. */
void MatchManager::createGameManagers() {
	AlgorithmRegistrar registrar = AlgorithmRegistrar::getInstance();
	auto algorithms = registrar.getAlgoFactoryVec();
	for (auto& algorithm : algorithms)
		 m_moveListVector.push_back(GameManager(m_name, m_maxSteps, m_rowsNum, m_colsNum,
			m_board, m_playerLocation, m_endLocation, move(algorithm())).play());
}

AlgorithmRegistrar AlgorithmRegistrar::instance;