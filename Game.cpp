#include "Game.h"
#include "math.h"
#include <random>
#include <time.h>

/********************************************************************************/
Game::Game(){
    gameBoard = new GameBoard();
    curent_turn = 0;
    fillBoard();
    initAIWeights();
}

/********************************************************************************/
Game::~Game(){

}

/********************************************************************************/
void Game::fillBoard(){
    Pos firstPlayerAngle = { 7, 0 };
    Pos secondPlayerAngle = { 0, 7 };
    Pos currentPos;
    uint32_t counter = 0;

    currentPos = firstPlayerAngle;
    for(uint32_t y = 0; y < 3; y++){
        for(uint32_t x = 0; x < 3; x++){
            gameBoard->setFigure(currentPos.px, currentPos.py, counter++, 0);
            currentPos.px--;
        }
        currentPos.px = firstPlayerAngle.px;
        currentPos.py++;
    }

    counter = 0;
    currentPos = secondPlayerAngle;
    for(uint32_t y = 0; y < 3; y++){
        for(uint32_t x = 0; x < 3; x++){
            gameBoard->setFigure(currentPos.px, currentPos.py, counter++, 1);
            currentPos.py--;
        }
        currentPos.py = secondPlayerAngle.py;
        currentPos.px++;
    }
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
        if(selectFigure()){
            toggleTurn();
        }
    }
}

/********************************************************************************/
void Game::initAIWeights(){
    uint32_t counter = 0;
    for(uint32_t y = 0; y < 8; y++){
        for(uint32_t x = 0; x < 8; x++){
            AI_weights[y][x] = 1;
        }
    }
}

/********************************************************************************/
void Game::recalcWeights(){

}

/********************************************************************************/
bool Game::selectFigure(){
    bool figure_selected = false;
    uint32_t collisions[4] = {0};
    uint32_t weights[4] = {0};

    Pos sPos;
    while(!figure_selected){
        gameBoard->deselectCell();
        srand(time(NULL));
        uint32_t figureId = rand()%8;
        if(gameBoard->findAndSelectCell(figureId, 1)){
            memcpy(collisions, getCollisions(), 4*sizeof(uint32_t));
            if(sum(collisions, 4) > 0) figure_selected = true;
        }
    }
    memcpy(weights, getWeights(collisions), 4*sizeof(uint32_t));
    
    sPos = gameBoard->getSelectedCellPosition();

    switch (max(weights, 4)){
        case 0: gameBoard->moveSelectedCell(sPos.px - 1, sPos.py); break;
        case 1: gameBoard->moveSelectedCell(sPos.px, sPos.py+1); break;
        case 2: gameBoard->moveSelectedCell(sPos.px + 1, sPos.py); break;
        case 3: gameBoard->moveSelectedCell(sPos.px, sPos.py-1); break;
    }

    return true;
}

/********************************************************************************/
uint32_t *Game::getCollisions(){
    static uint32_t collisions[4] = {0, 0, 0, 0};
    Pos curPos = gameBoard->getSelectedCellPosition();
    if(gameBoard->canIMoveCell(curPos.px-1, curPos.py)) collisions[0] = 1;
    if(gameBoard->canIMoveCell(curPos.px, curPos.py+1)) collisions[1] = 1;
    if(gameBoard->canIMoveCell(curPos.px+1, curPos.py)) collisions[2] = 1;
    if(gameBoard->canIMoveCell(curPos.px, curPos.py-1)) collisions[3] = 1;
    return collisions;
}

/********************************************************************************/
uint32_t *Game::getWeights(uint32_t *collisions){
    static uint32_t weights[4] = {0, 0, 0, 0};
    Pos curPos = gameBoard->getSelectedCellPosition();
    Pos newPos;

    newPos = { curPos.px - 1, curPos.py };
    if(isPositionCorrect(newPos))
    weights[0] = collisions[0] * AI_weights[newPos.px][newPos.py];
    newPos = { curPos.px, curPos.py+1 };
    if(isPositionCorrect(newPos))
    weights[1] = collisions[1] * AI_weights[newPos.px][newPos.py];
    newPos = { curPos.px + 1, curPos.py };
    if(isPositionCorrect(newPos))
    weights[2] = collisions[2] * AI_weights[newPos.px][newPos.py];
    newPos = { curPos.px, curPos.py-1 };
    if(isPositionCorrect(newPos))
    weights[3] = collisions[3] * AI_weights[newPos.px][newPos.py];

    return weights;
}

uint32_t Game::sum(uint32_t *array, uint32_t size){
    uint32_t result = 0;
    for(uint32_t i = 0; i < size; i++) result += array[i];
    return result;
}

uint32_t Game::max(uint32_t *array, uint32_t size){
    uint32_t maximum = 0;
    uint32_t max_pos = 0;
    for(uint32_t i = 0; i < size; i++){
        if(array[i] > maximum){
            maximum = array[i];
            max_pos = i;
        } 
    }
    return max_pos;
}