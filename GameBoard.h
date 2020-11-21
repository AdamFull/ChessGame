#ifndef __GAMEBOARD__
#define __GAMEBOARD__

#include "Cell.h"

typedef struct{
    uint32_t px, py;
} Pos;


class GameBoard{
public:
    GameBoard();
    ~GameBoard();

    Cell *getCell(uint32_t x, uint32_t y) { return &boardCells[y][x]; }

    bool selectCell(uint32_t x, uint32_t y, uint32_t owner_id);
    void deselectCell();
    bool isCellSelected() { return cellSelected; }
    Cell *getSelectedCell() { return selectedCell; }
    Pos getSelectedCellPosition() { return selCellPos; }

    void setFigure(uint32_t x, uint32_t y, uint32_t fig_id, uint32_t owner_id) { boardCells[y][x].placeFigure(fig_id, owner_id); }

    bool findAndSelectCell(uint32_t fig_id, uint32_t owner_id);

    bool moveSelectedCell(uint32_t x, uint32_t y);
    bool canIMoveCell(uint32_t x, uint32_t y);

private:
    Cell boardCells[8][8];
    Cell *selectedCell;
    Pos selCellPos;
    bool cellSelected;
};

#endif