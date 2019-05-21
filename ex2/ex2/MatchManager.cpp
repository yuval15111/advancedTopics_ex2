#include "MatchManager.h"
#include "AlgorithmRegistration.h"

/* -------------------------- GameManager main functions ---------------------------- */

/*	This function creates a GameManager obj for each factoryMethod in the registrar,
	and runs all games using their algorithms instantly using the GameManager play() function.
	Afterwards, the returned value (a MoveList object) is pushed to the instance MoveListVector. */
void MatchManager::createGameManagers() {
	vector<thread> threadVector;

	for (int i = 0; i < m_numOfThreads - 1; i++) {
		threadVector.emplace_back(thread(&MatchManager::threadFunc)); // TODO: ", this" ?
	}

	threadFunc(); // we use the main thread as one of the <m_numOfThreads> threads

	for (auto& t : threadVector) {
		t.join();
	}
}

void MatchManager::threadFunc() {
	while (true) {
		AlgorithmRegistrar registrar = AlgorithmRegistrar::getInstance();
		auto algorithm = getAlgorithmFromStack();
		if (algorithm == nullptr) return; // No more algorithms in the stack
		m_moveListVector.push_back(GameManager(m_name, m_maxSteps, m_rowsNum, m_colsNum, m_board,
						 m_playerLocation, m_endLocation, move(algorithm())).play());
	}
}

AlgorithmFactory MatchManager::getAlgorithmFromStack()
{
	m_mtx.lock();
	AlgorithmRegistrar registrar = AlgorithmRegistrar::getInstance();

	auto algorithmStack = registrar.getAlgoFactoryStack();
	if (algorithmStack.empty()) { 
		m_mtx.unlock();
		return nullptr;
	}

	AlgorithmFactory a = algorithmStack.top();
	algorithmStack.pop();
	m_mtx.unlock();
	return a;
}

AlgorithmRegistrar AlgorithmRegistrar::instance;
//mutex mtx;