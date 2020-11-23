#include "Game.h"
#include "math.h"
#include <random>
#include <time.h>
#include <stdio.h>
#include <cstring>
#include <queue>

/********************************************************************************/
Game::Game(){
    gameBoard = new GameBoard();
    AI = new AIPlayer(gameBoard);
    curent_turn = 0;
    gameBoard->installBoard();
    endgame = false;
}

/********************************************************************************/
Game::~Game(){
    delete gameBoard;
}

/********************************************************************************/
void Game::toggleTurn(){
    if(curent_turn == 0) curent_turn = 1;
    else curent_turn = 0;
}

//Ai methods
/********************************************************************************/
void Game::AI_Worker(){
    if(curent_turn == 1){
        Area aiturn = AI->getMove();
        gameBoard->selectCell(aiturn.start.px, aiturn.start.py, 1);
        gameBoard->moveSelectedCell(aiturn.end.px, aiturn.end.py);
        toggleTurn();
    }
}

/********************************************************************************/
uint32_t *Game::getCollisions(){
    static uint32_t collisions[4] = {0, 0, 0, 0};
    memset(collisions, 0, sizeof(uint32_t)*4);
    Pos curPos = gameBoard->getSelectedCellPosition();
    if(gameBoard->canIMoveCell(curPos.px-1, curPos.py)) collisions[0] = 1;
    if(gameBoard->canIMoveCell(curPos.px, curPos.py+1)) collisions[1] = 1;
    if(gameBoard->canIMoveCell(curPos.px+1, curPos.py)) collisions[2] = 1;
    if(gameBoard->canIMoveCell(curPos.px, curPos.py-1)) collisions[3] = 1;
    return collisions;
}