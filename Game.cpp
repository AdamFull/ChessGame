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
    fillBoard();
    initAIWeights();
}

/********************************************************************************/
Game::~Game(){
    delete gameBoard;
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
        Area aiturn = AI->getMove();
        gameBoard->selectCell(aiturn.start.px, aiturn.start.py, 1);
        gameBoard->moveSelectedCell(aiturn.end.px, aiturn.end.py);
        toggleTurn();
        /*if(selectFigure()){
            toggleTurn();

            for(uint32_t y = 0; y < 8; y++){
                for(uint32_t x = 0; x < 8; x++){
                    if(gameBoard->getCell(x, y)->isEmpty()){
                        printf("|O");
                    }else{
                        printf("|X");
                    }
                }
                printf("\n");
            }
        }*/
    }
}

/********************************************************************************/
void Game::initAIWeights(){
    uint32_t counter = 0;
    for(uint32_t y = 0; y < 8; y++){
        for(uint32_t x = 0; x < 8; x++){
            if(gameBoard->getCell(x, y)->getFigure()->getOwnerId() == 1) AI_weights[y][x] = 1;
            else if(gameBoard->getCell(x, y)->getFigure()->getOwnerId() == 0) AI_weights[y][x] = 99;
            else AI_weights[x][y] = counter;
            counter++;
        }
    }
}

/********************************************************************************/
void Game::recalcWeights(){
    
}

/********************************************************************************/
bool Game::selectFigure(){
    bool figure_selected = false;
    uint32_t AIcollisions[4] = {0};

    Pos sPos;
    while(!figure_selected){
        gameBoard->deselectCell();
        srand(time(NULL));
        uint32_t figureId = rand()%8;
        if(gameBoard->findAndSelectCell(figureId, 1)){
            memcpy(AIcollisions, getCollisions(), 4*sizeof(uint32_t));
            if(sum(AIcollisions, 4) > 0) figure_selected = true;
        }
    }
    printf("AI select: %d:%d\n", gameBoard->getSelectedCellPosition().px, gameBoard->getSelectedCellPosition().py);
    
    printf("Moves weights: left %d, top %d, right %d, down %d\n", AIcollisions[0], AIcollisions[1], AIcollisions[2], AIcollisions[3]);

    sPos = gameBoard->getSelectedCellPosition();

    switch (max(AIcollisions, 4)){
        case 0: gameBoard->moveSelectedCell(sPos.px - 1, sPos.py); break;
        case 1: gameBoard->moveSelectedCell(sPos.px, sPos.py+1); break;
        case 2: gameBoard->moveSelectedCell(sPos.px + 1, sPos.py); break;
        case 3: gameBoard->moveSelectedCell(sPos.px, sPos.py-1); break;
    }

    recalcWeights();

    return true;
}

/********************************************************************************/
uint32_t *Game::getCollisions(){
    static uint32_t collisions[4] = {0, 0, 0, 0};
    memset(collisions, 0, sizeof(uint32_t)*4);
    Pos curPos = gameBoard->getSelectedCellPosition();
    if(gameBoard->canIMoveCell(curPos.px-1, curPos.py)) {
        collisions[0] = 1 * calculateDistance({curPos.px-1, curPos.py}, {0, 7});
    }
    if(gameBoard->canIMoveCell(curPos.px, curPos.py+1)){
        collisions[1] = 1 * calculateDistance({curPos.px, curPos.py+1}, {0, 7});
    } 
    if(gameBoard->canIMoveCell(curPos.px+1, curPos.py))
    {
        collisions[2] = 1 * calculateDistance({curPos.px+1, curPos.py}, {0, 7});
    } 
    if(gameBoard->canIMoveCell(curPos.px, curPos.py-1)){
        collisions[3] = 1 * calculateDistance({curPos.px, curPos.py-1}, {0, 7});
    } 
    return collisions;
}

/********************************************************************************/
uint32_t *Game::getWeights(uint32_t *collisions){
    static uint32_t weights[4] = {0, 0, 0, 0};
    memset(weights, 0, sizeof(uint32_t)*4);
    Pos curPos = gameBoard->getSelectedCellPosition();
    Pos newPos;

    newPos = { curPos.px - 1, curPos.py };
    if(isPositionCorrect(newPos))
    weights[0] = collisions[0];
    newPos = { curPos.px, curPos.py+1 };
    if(isPositionCorrect(newPos))
    weights[1] = collisions[1];
    newPos = { curPos.px + 1, curPos.py };
    if(isPositionCorrect(newPos))
    weights[2] = collisions[2];
    newPos = { curPos.px, curPos.py-1 };
    if(isPositionCorrect(newPos))
    weights[3] = collisions[3];

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

uint32_t Game::min(uint32_t *array, uint32_t size){
    uint32_t minimum = array[0];
    uint32_t min_pos = 0;
    for(uint32_t i = 0; i < size; i++){
        if(array[i] < minimum){
            minimum = array[i];
            min_pos = i;
        } 
    }
    return min_pos;
}

int32_t Game::calculateDistance(Pos start, Pos end){
    bool visited[8][8];

    for(uint32_t y = 0; y < 8; y++){
        for (uint32_t x = 0; x < 8; x++){
            if(!gameBoard->getCell(x, y)->isEmpty()) visited[y][x] = true;
            else visited[y][x] = false;
        }
        
    }

    std::queue<Pos> qstart;
    qstart.push(start);
    visited[start.py][start.px] = true;
    while (!qstart.empty()) { 
        Pos cur = qstart.front();
        qstart.pop();

        // Destination found; 
        if (cur.px == end.px && cur.py == end.py) 
            return cur.dist; 
  
        // moving up 
		if (cur.py - 1 >= 0 && visited[cur.py - 1][cur.px] == false) { 
			qstart.push({cur.py - 1, cur.px, cur.dist + 1}); 
			visited[cur.py - 1][cur.px] = true; 
		} 

		// moving down 
		if (cur.py + 1 < 8 && visited[cur.py + 1][cur.px] == false) { 
			qstart.push({cur.py + 1, cur.px, cur.dist + 1}); 
			visited[cur.py + 1][cur.px] = true; 
		} 

		// moving left 
		if (cur.px - 1 >= 0 && visited[cur.py][cur.px - 1] == false) { 
			qstart.push({cur.py, cur.px - 1, cur.dist + 1}); 
			visited[cur.py][cur.px - 1] = true; 
		} 

		// moving right 
		if (cur.px + 1 < 8 && visited[cur.py][cur.px + 1] == false) { 
			qstart.push({cur.py, cur.px + 1, cur.dist + 1}); 
			visited[cur.py][cur.px + 1] = true; 
		} 
    }

    return (abs(start.px - end.px) + abs(start.py - end.py));
}