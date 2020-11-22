#include "AIPlayer.h"
#include "limits.h"
#include "cmath"
#include <queue>
#include <stdio.h>
#include <cstring>
#include <time.h>

/*******************************************************************************/
AIPlayer::AIPlayer(/* args */){
}

/*******************************************************************************/
AIPlayer::AIPlayer(GameBoard *gb){
    gameBoard = gb;
    /*gameBoard = (GameBoard*)calloc(1, sizeof(GameBoard));
    memcpy(gameBoard, gb, sizeof(GameBoard));*/
}

/*******************************************************************************/
AIPlayer::~AIPlayer(){
    free(gameBoard);
}

/************************************************************************************/
void AIPlayer::clearTree(std::vector<AIMove*> &tree){
	for(std::vector<AIMove *>::iterator i = tree.begin(); i != tree.end(); i++){
		AIMove *m = *i;
		if(m->next.size() != 0)
			clearTree(m->next);
		delete m;
	}
}

/************************************************************************************/
int32_t AIPlayer::calculateDistance(Pos start, Pos end){
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

/************************************************************************************/
Area AIPlayer::getMove(){
    std::vector<AIMove*> moveList;

    for(int i = 0; i < 9; i++){
		std::vector<AIMove*> moves = possibleMovesForPiece(gameBoard, i, NULL);
		moveList.insert(moveList.end(), moves.begin(), moves.end());
	}

    //Minimax tree
    std::vector<AIMove*> nowList = moveList;
	std::vector<AIMove*> nextList;

    for(int i = 0; i < this->maxDepth - 1; i++){
		//for each table in this level
		for(int j = 0; j < nowList.size(); j++){
			AIMove *m = nowList[j];
            GameBoard &board = m->gameBoard;
			//go through the pieces and generate a new level of movements
			for(int k = 0; k < 9; k++){
				std::vector<AIMove*> moves = possibleMovesForPiece(&board, k, m);
				nowList[j]->next.insert(nowList[j]->next.end(), moves.begin(), moves.end());
				nextList.insert(nextList.end(), moves.begin(), moves.end());
			}
		}
		//goes to the next level.
		nowList = nextList;
		nextList.clear();
	}

	AIMove *aiMove = getBestMove(moveList,0, this->maxDepth);
    Area newTurn = { aiMove->from, aiMove->to };
    printf("Move score: %d\n", aiMove->score);
    for(AIMove* m : moveList) {
        for(AIMove* mm : m->next){
            for(AIMove* mmm : mm->next){
                delete mmm;
                mm->next.clear();
            }
            delete mm;
            m->next.clear();
        }
        /*if(m->previous == nullptr){
            delete m;
            m->next.clear();
        }*/
    }
	this->clearTree(moveList);

    return newTurn;
}

/************************************************************************************/
std::vector<AIMove*> AIPlayer::possibleMovesForPiece(GameBoard *board, uint32_t fig_id, AIMove *parent){
    std::vector<AIMove*> moveList;
    Pos curPos;

    int dScore = 1;

    board->findAndSelectCell(fig_id, 1);

    //Analyse enemy positions

    curPos = board->getSelectedCellPosition();

    srand(time(NULL));

    //Move left
    if(board->canIMoveCell(curPos.px-1, curPos.py)){
        dScore = rand()%2;
        if(dScore != 1) dScore = -1;
        dScore *= calculateDistance({curPos.px-1, curPos.py}, {0, 7});
        if(curPos.px == 7 && curPos.py == 0) dScore = 0;
        GameBoard newBoard(board);
        newBoard.selectCell(curPos.px, curPos.py, 1);
        newBoard.moveSelectedCell(curPos.px-1, curPos.py);
        AIMove *pMove = new AIMove(curPos, {curPos.px-1, curPos.py}, 1 * dScore, parent, newBoard);
		moveList.push_back(pMove);
    }

    //Move top
    if(board->canIMoveCell(curPos.px, curPos.py+1)){
        dScore = rand()%2;
        if(dScore != 1) dScore = -1;
        dScore *= calculateDistance({curPos.px, curPos.py+1}, {0, 7});
        if(curPos.px == 7 && curPos.py == 0) dScore = 0;
        GameBoard newBoard(board);
        newBoard.selectCell(curPos.px, curPos.py, 1);
        newBoard.moveSelectedCell(curPos.px, curPos.py+1);
        AIMove *pMove = new AIMove(curPos, {curPos.px, curPos.py+1}, 1 * dScore, parent, newBoard);
		moveList.push_back(pMove);
    }

    //Move right
    if(board->canIMoveCell(curPos.px+1, curPos.py)){
        dScore = rand()%2;
        if(dScore != 1) dScore = -1;
        dScore *= calculateDistance({curPos.px+1, curPos.py}, {0, 7});
        if(curPos.px == 7 && curPos.py == 0) dScore = 0;
        GameBoard newBoard(board);
        newBoard.selectCell(curPos.px, curPos.py, 1);
        newBoard.moveSelectedCell(curPos.px+1, curPos.py);
        AIMove *pMove = new AIMove(curPos, {curPos.px+1, curPos.py}, 1 * dScore, parent, newBoard);
		moveList.push_back(pMove);
    }

    //Move down
    if(board->canIMoveCell(curPos.px, curPos.py-1)){
        dScore = rand()%2;
        if(dScore != 1) dScore = -1;
        dScore *= calculateDistance({curPos.px, curPos.py-1}, {0, 7});
        if(curPos.px == 0 && curPos.py == 7) dScore = 0;
        GameBoard newBoard(board);
        newBoard.selectCell(curPos.px, curPos.py, 1);
        newBoard.moveSelectedCell(curPos.px, curPos.py-1);
        AIMove *pMove = new AIMove(curPos, {curPos.px, curPos.py-1}, 1 * dScore, parent, newBoard);
		moveList.push_back(pMove);
    }

    return moveList;
}

/************************************************************************************/
AIMove *AIPlayer::getBestMove(std::vector<AIMove*> &moveList, int layer, int maxLayer){
/////////////////////////////////////////
//	Go through all layers and find min //
//									   //
//		__*__						   //
//	   *     *   ------ layer 2		   //
//	  / \   / \						   //
//   *   * *   * ------ layer 3        //
//									   //
/////////////////////////////////////////
	if(layer == maxLayer - 1){
		//if we are in the last level we calculate the minimum
		//and propagate it to the parent
		int min = INT_MAX;
		for(int j = 0; j < moveList.size(); j++){
			if(moveList[j]->score < min)
				min = moveList[j]->score;
		}
		moveList[0]->previous->score += min;
		if(maxLayer != 1){
			return NULL;
		}
	}else{
		//if we're not in the last level we need to propagate
		//the minimum calculation first
		for(int j = 0; j < moveList.size(); j++)
			getBestMove(moveList[j]->next,layer + 1, maxLayer);
		int min = INT_MAX;
		for(int j = 0; j < moveList.size(); j++){
			if(moveList[j]->score < min)
				min = moveList[j]->score;
		}

		if(moveList[0]->previous != NULL)
			moveList[0]->previous->score += min;
	}

	if(layer == 0){
		AIMove *retMove;
		int maxV = INT_MIN;
		for(int i = 0; i < moveList.size(); i++)
			if(moveList[i]->score > maxV){
				maxV = moveList[i]->score;
				retMove = moveList[i];
			}
		return retMove;
	}
	return NULL;
}

/************************************************************************************/
AIMove* AIPlayer::createTree(int treeDepth){
	AIMove *gameTree;
	bool aiPlay = true;
	for(int i = 0; i < treeDepth; i++){

		//vector<Piece> &pieces;
		
		
		//flip player
		aiPlay = !aiPlay;
	}

	return gameTree;
}