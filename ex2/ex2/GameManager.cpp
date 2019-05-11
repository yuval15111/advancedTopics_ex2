#include "GameManager.h"

/* -------------------------- GameManager main functions ---------------------------- */

/*	This function emulates the GameManager's algorithm.
	return: A MoveList object, contains all the moves the GameManager's algorithm did. */
MoveList GameManager::play() {
	MoveList moveList;
	int seq;
	for (int i = 1; i <= m_maxSteps; ++i) {
		Move m = m_algorithm->move();							// PLAYER: THIS IS MY MOVE!
		moveList.push_back(getMoveChar(m));
		if (m == Move::BOOKMARK) updateBookmark();
		else {
			execute(m);											// MANAGER: OK, LET ME WRITE THAT DOWN...
			if (playerHitsEndChar()) {
				//printWinMessage(i);							// MANAGER: OMG YOU DID IT! I ALWAYS BELIEVED IN YOU
				moveList.push_back('!');
				return moveList;
			}
			else if (playerHitsWallChar()) {
				m_algorithm->hitWall();							// PLAYER: OUCH!!
				execute(m, true); 								// MANAGER: SORRY PAL, TRY AGAIN
			}
			else if ((seq = playerHitsBookmark()) != -1) {		// MANAGER: YOU'RE RIGHT... HERE! <POINTING AT MAP>
				m_algorithm->hitBookmark(seq);					// PLAYER: OHHH I REMEMBER THAT PLACE!
			}
		}
	}
	moveList.push_back('X');
	//printLostMessage(m_maxSteps);								// MANAGER: YOU SHOULD TRY HARDER NEXT TIME. CYA!
	return moveList;
}

/*	This function is responsible of updating the player's actual location in the maze.
	params: A Move <a>, and a boolean <undo>.
	if undo == true, then the player will move to its previous position. */
void GameManager::execute(Move a, const bool undo) {
	a = undo ? !a : a;
	switch (a) {
	case Move::UP:
		m_playerLocation.first += m_rowsNum - 1;
		m_playerLocation.first %= m_rowsNum;
		break;
	case Move::DOWN:
		m_playerLocation.first++;
		m_playerLocation.first %= m_rowsNum;
		break;
	case Move::LEFT:
		m_playerLocation.second += m_colsNum - 1;
		m_playerLocation.second %= m_colsNum;
		break;
	case Move::RIGHT:
		m_playerLocation.second++;
		m_playerLocation.second %= m_colsNum;
		break;
	default:
		break;
	}
}

