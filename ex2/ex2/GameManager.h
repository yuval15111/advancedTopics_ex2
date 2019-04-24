#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "MainAux.h"

class GameManager {
public:

private:
	string 					m_name;
	int 					m_maxSteps, m_rowsNum, m_colsNum;
	MazeBoard 				m_board;
	Coordinate 				m_playerLocation;
	Coordinate				m_endLocation;
	Coordinate				m_bookmark;
	//Player *				m_player = nullptr;

public:
	GameManager(string name, int maxSteps, int rowsNum, int colsNum,
		MazeBoard board, Coordinate playerLocation, Coordinate endLocation) :
		m_name(name), m_maxSteps(maxSteps), m_rowsNum(rowsNum),
		m_colsNum(colsNum), m_board(board), m_playerLocation(playerLocation),
		m_endLocation(endLocation), m_bookmark(playerLocation) {};
	~GameManager();

	inline void				updateBookmark() { m_bookmark = m_playerLocation; }
	void					execute(Move a, const bool undo = false);
	inline char				getCoordValue() { return m_board[m_playerLocation.first][m_playerLocation.second]; }
	inline bool				playerHitsEndChar() { return getCoordValue() == END_CHAR; }
	inline bool				playerHitsWallChar() { return getCoordValue() == WALL_CHAR; }
	inline bool				playerHitsBookmark() { return m_playerLocation == m_bookmark; }
	inline int				getMaxSteps() { return m_maxSteps; }
	vector<char>			play();
};

#endif