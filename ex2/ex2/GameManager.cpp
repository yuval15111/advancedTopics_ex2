#include "GameManager.h"

GameManager::~GameManager()
{
	delete m_player;
}

/* main function of manager, where the game flow is */
vector<char> Manager::play() {
	vector<char> actionsLst;
	m_player = new Player();										// PLAYER: LET'S GO!
	for (int i = 1; i <= m_maxSteps; ++i) {
		Action action = m_player->move();							// PLAYER: THIS IS MY MOVE!
		actionsLst.push_back(getActionChar(action));
		if (action == Action::BOOKMARK)	updateBookmark();
		else {
			execute(action);										// MANAGER: OK, LET ME WRITE THAT DOWN...
			if (playerHitsEndChar()) {
				printWinMessage(i);									// MANAGER: OMG YOU DID IT! I ALWAYS BELIEVED IN YOU
				actionsLst.push_back('!');
				return actionsLst;
			}
			else if (playerHitsWallChar()) {
				m_player->hitWall();								// PLAYER: OUCH!!
				execute(action, true); 								// MANAGER: SORRY PAL, TRY AGAIN
			}
			else if (playerHitsBookmark()) {						// MANAGER: YOU'RE RIGHT... HERE! <POINTING AT MAP>
				m_player->hitBookmark();							// PLAYER: OHHH I REMEMBER THAT PLACE!
			}
		}
	}
	actionsLst.push_back('X');
	printLostMessage(m_maxSteps);									// MANAGER: YOU SHOULD TRY HARDER NEXT TIME. CYA!
	return actionsLst;
}

void Manager::execute(Action a, const bool undo)
{
	a = undo ? !a : a;
	switch (a) {
	case Action::UP:
		m_playerLocation.first += m_rowsNum - 1;
		m_playerLocation.first %= m_rowsNum;
		break;
	case Action::DOWN:
		m_playerLocation.first++;
		m_playerLocation.first %= m_rowsNum;
		break;
	case Action::LEFT:
		m_playerLocation.second += m_colsNum - 1;
		m_playerLocation.second %= m_colsNum;
		break;
	case Action::RIGHT:
		m_playerLocation.second++;
		m_playerLocation.second %= m_colsNum;
		break;
	default:
		break;
	}
}

