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
    

private:
    GameBoard *gameBoard;
    AIPlayer *AI;
    uint32_t curent_turn;
    uint32_t AI_weights[8][8];

private:
    void fillBoard();
    void initAIWeights();
    void recalcWeights();
    bool selectFigure();
public:
    uint32_t *getCollisions();
    uint32_t *getWeights(uint32_t *collisions);
private:
    uint32_t sum(uint32_t *array, uint32_t size);
    uint32_t max(uint32_t *array, uint32_t size);
    uint32_t min(uint32_t *array, uint32_t size);
    int32_t calculateDistance(Pos start, Pos end);
    bool isPositionCorrect(Pos position) { return (position.px < 8 && position.py < 8); }
};

#endif