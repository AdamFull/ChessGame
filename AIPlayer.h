#ifndef __AIPLAYER__
#define __AIPLAYER__

#include "AIMove.h"

typedef struct{
    Pos start, end;
} Area;

class AIPlayer{
private:
    const size_t maxDepth = 3;
    const int maxPathDepth = 10;
    void clearTree(std::vector<AIMove*> *tree);
    int32_t calcShortDistance(Pos start, Pos end);
    int32_t calcLongDistance(Pos start, Pos end);
    GameBoard *gameBoard;
public:
    AIPlayer(/* args */);
    AIPlayer(GameBoard *gb);
    ~AIPlayer();

    Area getMove();

    std::vector<AIMove*> possibleMovesForPiece(GameBoard *board,uint32_t fig_id, AIMove *parent);
    int32_t calcMotivator(Pos newPos, int32_t score);
    int32_t calcDemotivator(int32_t lScore, int32_t sScore);

	AIMove *createTree(int treeDepth);

	AIMove *getBestMove (std::vector<AIMove*> &moveList, int layer, int maxLayer);
};


#endif