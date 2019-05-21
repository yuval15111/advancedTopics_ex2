#include "MatchManager.h"
#include "AlgorithmRegistration.h"

/* -------------------------- GameManager main functions ---------------------------- */

/*	This function creates a GameManager obj for each factoryMethod in the registrar,
	and runs all games using their algorithms instantly using the GameManager play() function.
	Afterwards, the returned value (a MoveList object) is pushed to the instance MoveListVector. */
void MatchManager::createGameManagers() {
	vector<thread> threadVector;
	cout << "MM: fillStack()" << endl;
	//AlgorithmRegistrar::getInstance().fillStack();
	fillStack();
	cout << "MM: fillStack() end" << endl;
	cout << "MM: beginning of createGameManagers()" << endl;
	for (int i = 0; i < m_numOfThreads - 1; i++) {
		threadVector.emplace_back(thread(&MatchManager::threadFunc, this)); // TODO: ", this" ?
	}

	threadFunc(); // we use the main thread as one of the <m_numOfThreads> threads
	cout << "MM: after running of all the threads" << endl;
	for (auto& t : threadVector) {
		t.join();
	}
	cout << "MM: after finishing all the threads" << endl;
}

void MatchManager::threadFunc() {
	while (true) {
		cout << "MM: inside some threadFunc(). id = " << this_thread::get_id() << endl;
		cout << "MM: stack size before: " << m_AlgorithmStack.size() << endl;
		auto algorithm = getAlgorithmFromStack();
		cout << "MM: stack size after: " << m_AlgorithmStack.size() << endl;
		if (algorithm == nullptr) { 
			cout << "MM: end of some threadFunc() - empty stack" << endl;
			return; // No more algorithms in the stack
		} 
		m_moveListVector.push_back(GameManager(m_name, m_maxSteps, m_rowsNum, m_colsNum, m_board,
						 m_playerLocation, m_endLocation, move(algorithm())).play());
	}
}

AlgorithmFactory MatchManager::getAlgorithmFromStack() {
	m_mtx.lock();
	if (m_AlgorithmStack.empty()) {
		m_mtx.unlock();
		cout << "MM: end of getAlgorithmFromStack() - empty" << endl;
		return nullptr;
	}
	AlgorithmFactory a = m_AlgorithmStack.top().second;
	m_AlgorithmStack.pop();
	m_mtx.unlock();
	cout << "MM: end of getAlgorithmFromStack()" << endl;
	return a;
}

void MatchManager::fillStack() {
	AlgorithmRegistrar registrar = AlgorithmRegistrar::getInstance();
	vector<AlgorithmFactory>& vec = registrar.getAlgoFactoryVec();
	for (auto& a : vec) m_AlgorithmStack.push(make_pair("fdsgdfgv",a)); // TODO: change string
}

AlgorithmRegistrar AlgorithmRegistrar::instance;
//mutex mtx;