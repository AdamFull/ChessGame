#ifndef __AIMOVE__
#define __AIMOVE__

#include <vector>
#include "GameBoard.h"

class AIMove{
public:
    AIMove *previous;
	std::vector<AIMove*> next;
    int score;


    Pos from, to;

    GameBoard gameBoard;

    AIMove(Pos from, Pos to, int score, AIMove *parent, GameBoard gb);
};


#endif