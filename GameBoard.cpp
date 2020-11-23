#include "GameBoard.h"

#include <math.h>
#include "stdio.h"

/************************************************************************************************/
GameBoard::GameBoard(){
    cellSelected = false;
    selCellPos = {0, 0, 0};
}

GameBoard::GameBoard(GameBoard *gb){
    cellSelected = false;
    selCellPos = {0, 0, 0};
    boardCells = gb->boardCells;
}

/************************************************************************************************/
GameBoard::~GameBoard(){
}

/************************************************************************************************/
void GameBoard::installBoard(){
    uint32_t counter = 0;
    for(Pos pos : firstPlayerPositions){
        setFigure(pos.px, pos.py, counter++, 0);
    }

    counter = 0;
    for(Pos pos : secondPlayerPositions){
        setFigure(pos.px, pos.py, counter++, 1);
    }
}

/************************************************************************************************/
uint32_t GameBoard::getPlayerScore(uint32_t player_id){
    uint32_t counter = 0;

    if(player_id == 0){
        for(Pos pos : firstPlayerPositions){
            if(isEnemyCell(pos.px, pos.py, 1)){
                counter++;
            }
        }
    }else{
        for(Pos pos : secondPlayerPositions){
            if(isEnemyCell(pos.px, pos.py, 0)){
                counter++;
            }
        }
    }
    return counter;
}

/************************************************************************************************/
void GameBoard::setFigure(int32_t x, int32_t y, int32_t fig_id, int32_t owner_id) {
    boardCells[y][x].setCellPosition(x, y); 
    boardCells[y][x].placeFigure(fig_id, owner_id);
}

/************************************************************************************************/
bool GameBoard::selectCell(int32_t x, int32_t y, int32_t owner_id) { 
    selectedCell = boardCells[y][x];
    if(selectedCell.getFigure()->getOwnerId() == owner_id){
        if(!selectedCell.isEmpty()){
            selCellPos = {x, y, 0};
            cellSelected = true;
            return true;
        }else{
            deselectCell();
            return false;
        }
    }
    return false;
}

bool GameBoard::selectCell(Pos newPos, int32_t owner_id) { 
    return selectCell(newPos.px, newPos.py, owner_id);
}

/************************************************************************************************/
void GameBoard::deselectCell() { 
    cellSelected = false;
    selCellPos = {0, 0, 0};
}

bool GameBoard::findAndSelectCell(int32_t fig_id, int32_t owner_id){
    for(int32_t y = 0; y < 8; y++){
        for(int32_t x = 0; x < 8; x++){
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
std::vector<Cell> GameBoard::getCells(int32_t owner_id){
    std::vector<Cell> output;
    for(int32_t y = 0; y < 8; y++){
        for(int32_t x = 0; x < 8; x++){
            if(!boardCells[y][x].isEmpty()){
                Figure currentFigure = *boardCells[y][x].getFigure();
                if(currentFigure.getOwnerId() == owner_id){
                    output.push_back(boardCells[y][x]);
                }
            }
        }
    }
    return output;
}

/************************************************************************************************/
bool GameBoard::moveSelectedCell(int32_t x, int32_t y){
    if(canIMoveCell(x, y)){
        boardCells[selCellPos.py][selCellPos.px].replaceFigure();
        boardCells[y][x].placeFigure(selectedCell.getFigure());
        boardCells[y][x].setCellPosition(x, y);
        deselectCell();
        return true;
    }
    return false;
}

/************************************************************************************************/
bool GameBoard::moveSelectedCell(Pos newPos){
    return moveSelectedCell(newPos.px, newPos.py);
}

/************************************************************************************************/
/*************************************Check game rule********************************************/
/************************************************************************************************/
bool GameBoard::canIMoveCell(int32_t x, int32_t y){
    if(x > 7 || y > 7 || x < 0 || y < 0) return false;
    Pos new_position = {abs((int)selCellPos.px - (int)x), abs((int)selCellPos.py - (int)y) };
    //Checking is cell empty
    if(!boardCells[y][x].isEmpty()) return false;
    if(new_position.px > 1 || new_position.py > 1) return false;
    if(new_position.px == 1 && new_position.py == 1) return false;
    return true;
}

/************************************************************************************************/
bool GameBoard::canIMoveCell(Pos newPos){
    return canIMoveCell(newPos.px, newPos.py);
}

/************************************************************************************************/
bool GameBoard::isEnemyCell(int32_t x, int32_t y, int32_t owner_id){
    if(x > 7 || y > 7 || x < 0 || y < 0) return false;
    if(boardCells[y][x].isEmpty()) return false;
    int32_t cur_id = boardCells[y][x].getFigure()->getOwnerId();
    if(cur_id == owner_id) return true;
    return false;
}

int32_t GameBoard::getCellWeight(Pos cellPos){
    for(Pos cur : firstPlayerPositions){
        if(cur.px == cellPos.px && cur.py == cellPos.py) return cur.dist;
    }
    return 1;
}