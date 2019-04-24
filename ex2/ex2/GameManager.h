#pragma once

#include "AbstractAlgorithm.h"

class GameManager : public AbstractAlgorithm {
public:
	Move move();
	void hitWall();
	void hitBookmark(int seq);
};