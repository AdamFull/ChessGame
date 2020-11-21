#include "GameBoard.h"

#include <math.h>

/************************************************************************************************/
GameBoard::GameBoard(){
    cellSelected = false;
    selCellPos = {0, 0};
}

/************************************************************************************************/
GameBoard::~GameBoard(){
}

/************************************************************************************************/
bool GameBoard::selectCell(uint32_t x, uint32_t y, uint32_t owner_id) { 
    selectedCell = &boardCells[y][x]; 
    if(selectedCell->getFigure()->getOwnerId() == owner_id){
        if(!selectedCell->isEmpty()){
            selCellPos = {x, y};
            cellSelected = true;
            return true;
        }else{
            deselectCell();
            return false;
        }
    }
    return false;
}

/************************************************************************************************/
void GameBoard::deselectCell() { 
    cellSelected = false;
    selCellPos = {0, 0};
}

bool GameBoard::findAndSelectCell(uint32_t fig_id, uint32_t owner_id){
    for(uint32_t y = 0; y < 8; y++){
        for(uint32_t x = 0; x < 8; x++){
            if(!boardCells[y][x].isEmpty()){
                Figure currentFigure = *boardCells[y][x].getFigure();
                if(currentFigure.getId() == fig_id && currentFigure.getOwnerId() == owner_id){
                    selectCell(x, y, owner_id);
                    return true;
                }
            }
        }
    }
    return false;
}

/************************************************************************************************/
bool GameBoard::moveSelectedCell(uint32_t x, uint32_t y){
    if(canIMoveCell(x, y)){
        boardCells[selCellPos.py][selCellPos.px].replaceFigure();
        boardCells[y][x].placeFigure(selectedCell->getFigure());
        deselectCell();
        return true;
    }
    return false;
}

/************************************************************************************************/
/*************************************Check game rule********************************************/
/************************************************************************************************/
bool GameBoard::canIMoveCell(uint32_t x, uint32_t y){
    if(x > 7 || y > 7) return false;
    Pos new_position = {abs((int)selCellPos.px - (int)x), abs((int)selCellPos.py - (int)y) };
    //Checking is cell empty
    if(!boardCells[y][x].isEmpty()) return false;
    if(new_position.px > 1 || new_position.py > 1) return false;
    if(new_position.px == 1 && new_position.py == 1) return false;
    return true;
}