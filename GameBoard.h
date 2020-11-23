#ifndef __GAMEBOARD__
#define __GAMEBOARD__

#include "Cell.h"
#include <vector>
#include <array>

class GameBoard{
public:
    GameBoard();
    GameBoard(GameBoard *gb);
    ~GameBoard();

    void installBoard();
    uint32_t getPlayerScore(uint32_t player_id);

    Cell *getCell(int32_t x, int32_t y) { return &boardCells[y][x]; }
    Cell *getCell(Pos cPos) { return &boardCells[cPos.py][cPos.px]; }

    bool selectCell(int32_t x, int32_t y, int32_t owner_id);
    bool selectCell(Pos newPos, int32_t owner_id);

    void deselectCell();
    bool isCellSelected() { return cellSelected; }
    Cell *getSelectedCell() { return &selectedCell; }
    Pos getSelectedCellPosition() { return selCellPos; }

    void setFigure(int32_t x, int32_t y, int32_t fig_id, int32_t owner_id);

    bool findAndSelectCell(int32_t fig_id, int32_t owner_id);

    std::vector<Cell> getCells(int32_t owner_id);

    bool moveSelectedCell(int32_t x, int32_t y);
    bool moveSelectedCell(Pos newPos);

    bool canIMoveCell(int32_t x, int32_t y);
    bool canIMoveCell(Pos newPos);
    bool isEnemyCell(int32_t x, int32_t y, int32_t owner_id);

    int32_t getCellWeight(Pos cellPos);

private:
    std::array<std::array<Cell, 8>, 8> boardCells;
    Cell selectedCell;
    Pos selCellPos;
    bool cellSelected;

private:
    std::vector<Pos> firstPlayerPositions = { {7, 0, -9}, {7, 1, -8}, {7, 2, -7},
                                              {6, 0, -6}, {6, 1, -5}, {6, 2, -4},
                                              {5, 0, -3}, {5, 1, -2}, {5, 2, -1} };
    std::vector<Pos> secondPlayerPositions = { {0, 7, 9}, {1, 7, 8}, {2, 7, 7},
                                               {0, 6, 6}, {1, 6, 5}, {2, 6, 4},
                                               {0, 5, 3}, {1, 5, 2}, {2, 5, 1} };
};

#endif