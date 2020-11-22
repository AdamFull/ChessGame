#ifndef __AIPLAYER__
#define __AIPLAYER__

#include "AIMove.h"

typedef struct{
    Pos start, end;
} Area;

class AIPlayer{
private:
    const int maxDepth = 3;
    const int maxPathDepth = 10;
    void clearTree(std::vector<AIMove*> &tree);
    int32_t calculateDistance(Pos start, Pos end);
    GameBoard *gameBoard;
public:
    AIPlayer(/* args */);
    AIPlayer(GameBoard *gb);
    ~AIPlayer();

    Area getMove();

    std::vector<AIMove*> possibleMovesForPiece(GameBoard *board,uint32_t fig_id, AIMove *parent);

	AIMove *createTree(int treeDepth);

	AIMove *getBestMove (std::vector<AIMove*> &moveList, int layer, int maxLayer);
};


#endif