#ifndef __GAME__
#define __GAME__

#include "stdint.h"
#include "AIPlayer.h"

class Game{
public:
    Game();
    ~Game();

    GameBoard *getGameBoard() { return gameBoard; }

    void toggleTurn();

    void AI_Worker();

    void endthisgame() { endgame = true; }
    bool getEndgame() { return endgame; }
    

private:
    GameBoard *gameBoard;
    AIPlayer *AI;
    uint32_t curent_turn;
    bool endgame;

private:
    void fillBoard();
    void initAIWeights();
    void recalcWeights();
    bool selectFigure();
public:
    uint32_t *getCollisions();
};

#endif