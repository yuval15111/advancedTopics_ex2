#include "MatchManager.h"
#include "AlgorithmRegistration.h"

/* -------------------------- GameManager main functions ---------------------------- */

/*	This function creates a GameManager obj for each factoryMethod in the registrar,
	and runs all games using their algorithms instantly using the GameManager play() function.
	Afterwards, the returned value (a MoveList object) is pushed to the instance MoveListVector. */
void MatchManager::createGameManagers() {
	vector<thread> threadVector;
	fillStack();
	for (int i = 0; i < m_numOfThreads - 1; i++)
		threadVector.emplace_back(thread(&MatchManager::threadFunc, this));

	threadFunc(); // we use the main thread as one of the <m_numOfThreads> threads
	for (auto& t : threadVector) t.join();
}

void MatchManager::threadFunc() {
	while (true) {
		pair<string, AlgorithmFactory> p = getAlgorithmFromStack();
		if (p.second == nullptr) return; // No more algorithms in the stack
		m_moveListMap[p.first] = GameManager(m_name, m_maxSteps, m_rowsNum, m_colsNum, m_board,
						 m_playerLocation, m_endLocation, move(p.second())).play();
	}
}

pair<string, AlgorithmFactory> MatchManager::getAlgorithmFromStack() {
	m_mtx.lock();
	if (m_AlgorithmStack.empty()) {
		m_mtx.unlock();
		return make_pair("", nullptr);
	}
	pair<string, AlgorithmFactory> a = m_AlgorithmStack.top();
	m_AlgorithmStack.pop();
	m_mtx.unlock();
	return a;
}

void MatchManager::fillStack() {
	AlgorithmRegistrar registrar = AlgorithmRegistrar::getInstance();
	vector<AlgorithmFactory>& vec = registrar.getAlgoFactoryVec();
	for (unsigned int i = 0; i < vec.size(); i++)
		m_AlgorithmStack.push(make_pair(m_algorithmNameVector[i], vec[i]));
}

AlgorithmRegistrar AlgorithmRegistrar::instance;