#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "MainAux.h"

class GameManager {

private:

	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------- GameManager members --------------------------------- */
	/* ---------------------------------------------------------------------------------------- */

	string 										m_name;
	int 										m_maxSteps, m_rowsNum, m_colsNum;
	MazeBoard 									m_board;
	Coordinate 									m_playerLocation;
	Coordinate									m_endLocation;
	map<Coordinate, int>						m_bookmarkMap;
	int											m_bookmarkCount = 0;
	unique_ptr<AbstractAlgorithm> 				m_algorithm;

public:

	/* ---------------------------------------------------------------------------------------- */
	/* ----------------------------- GameManager public functions ----------------------------- */
	/* ---------------------------------------------------------------------------------------- */

	/* ------------------------------------- c'tor -------------------------------------- */

	GameManager(string name, int maxSteps, int rowsNum, int colsNum, MazeBoard board, Coordinate playerLocation,
		Coordinate endLocation, unique_ptr<AbstractAlgorithm> algorithm) :
		m_name(name), m_maxSteps(maxSteps), m_rowsNum(rowsNum),
		m_colsNum(colsNum), m_board(board), m_playerLocation(playerLocation),
		m_endLocation(endLocation), m_bookmarkMap(), m_algorithm(move(algorithm)) {	};

	/* -------------------------- GameManager main functions ---------------------------- */

	MoveList				play();
	void					execute(Move a, const bool undo = false);

	/* ---------------------------- Other helper functions ------------------------------ */

	inline void				updateBookmark		() { m_bookmarkMap[m_playerLocation] = m_bookmarkCount++; }
	inline char				getCoordValue		() { return m_board[m_playerLocation.first][m_playerLocation.second]; }
	inline bool				playerHitsEndChar	() { return getCoordValue() == END_CHAR; }
	inline bool				playerHitsWallChar	() { return getCoordValue() == WALL_CHAR; }
	inline int				playerHitsBookmark	()
												   { return m_bookmarkMap.find(m_playerLocation) != m_bookmarkMap.end() ?
															m_bookmarkMap[m_playerLocation] : -1; }
	inline int				getMaxSteps			() { return m_maxSteps; }
};

#endif