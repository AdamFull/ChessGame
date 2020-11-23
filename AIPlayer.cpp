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
}

/*******************************************************************************/
AIPlayer::~AIPlayer(){
    free(gameBoard);
}

/************************************************************************************/
void AIPlayer::clearTree(std::vector<AIMove*> *tree){
	for(std::vector<AIMove *>::iterator i = tree->begin(); i != tree->end(); i++){
		AIMove *m = *i;
		if(m->next.size() != 0)
			clearTree(&m->next);
		delete m;
	}
}

int32_t randrange(int32_t max){
    return rand()%max > max/2 ? 1 : -1;
}

/************************************************************************************/
int32_t AIPlayer::calcShortDistance(Pos start, Pos end){
    Pos target = end;
    Pos curPos = target;
    std::vector<Pos> freePos;
    for(uint32_t y = 0; y < 3; y++){
        for(uint32_t x = 0; x < 3; x++){
            if(gameBoard->getCell(curPos.px, curPos.py)->isEmpty()){
                freePos.push_back(curPos);
            }
            curPos.py--;
        }
        curPos.py = target.py;
        curPos.px++;
    }

    for(Pos cur : freePos){
        target = cur;
        break;
    }
    // 
    return (abs(start.py - target.py - rand()%6) + abs(start.px - target.px +rand()%6));
}

int32_t AIPlayer::calcLongDistance(Pos start, Pos end){
    bool visited[8][8]; 
    for (int y = 0; y < 8; y++) { 
        for (int x = 0; x < 8; x++) { 
            //if (!gameBoard->getCell(i, j)->isEmpty()) visited[i][j] = true; 
            //if(gameBoard->isEnemyCell(i, j, 0)) visited[i][j] = true;
            visited[y][x] = false; 
        } 
    }

    std::queue<Pos> q; 
    q.push(start); 
    visited[start.px][start.py] = true; 

    /*for (int y = 0; y < 8; y++) { 
        for (int x = 0; x < 8; x++) {
            printf("|%d", (int)visited[y][x]) ;
        }
        printf("\n");
    }*/

    while (!q.empty()) { 
        Pos p = q.front(); 
        q.pop(); 
  
        // Destination found; 
        if (p.px == end.px && p.py == end.py) 
            return p.dist; 
  
        // moving up 
        if (p.py + 1 < 8 && 
            visited[p.py + 1][p.px] == false) { 
            q.push({p.py + 1, p.px, p.dist + 1}); 
            visited[p.py + 1][p.px] = true; 
        } 
  
        // moving down 
        if (p.py - 1 >= 0 && 
            visited[p.py - 1][p.px] == false) { 
            q.push({p.py - 1, p.px, p.dist + 1}); 
            visited[p.py - 1][p.px] = true; 
        } 
  
        // moving left 
        if (p.px - 1 >= 0 && 
            visited[p.py][p.px - 1] == false) { 
            q.push({p.py, p.px - 1, p.dist + 1}); 
            visited[p.py][p.px - 1] = true; 
        } 
  
         // moving right 
        if (p.px + 1 < 8 && 
            visited[p.py][p.px + 1] == false) { 
            q.push({p.py, p.px + 1, p.dist + 1}); 
            visited[p.py][p.px + 1] = true; 
        } 
    } 
    return -1; 
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

    for(size_t i = 0; i < this->maxDepth - 1; i++){
		//for each table in this level
		for(size_t j = 0; j < nowList.size(); j++){
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
	this->clearTree(&moveList);

    return newTurn;
}

/************************************************************************************/
std::vector<AIMove*> AIPlayer::possibleMovesForPiece(GameBoard *board, uint32_t fig_id, AIMove *parent){
    std::vector<AIMove*> moveList;
    Pos curPos;

    int dDemot = 1;
    int dMot = 1;

    board->findAndSelectCell(fig_id, 1);

    //Analyse enemy positions

    curPos = board->getSelectedCellPosition();

    const Pos positions[4] = {{curPos.px-1, curPos.py, 0}, {curPos.px, curPos.py+1, 0}, {curPos.px+1, curPos.py, 0}, {curPos.px, curPos.py-1, 0}};

    for(Pos newPos : positions){
        srand(time(NULL));
        if(board->canIMoveCell(newPos)){
            //Calculate motivator and demotivator
            int new_short_distance = calcShortDistance(newPos, {0, 7, 0});
            int new_long_distance = calcLongDistance(newPos, {0, 7, 0});
            //dDemot = calcDemotivator(new_long_distance, new_short_distance);
            //dMot = calcMotivator(newPos, new_long_distance);
            //dMot = abs(dMot*dDemot/maxPathDepth);
            //Ааа, почему без рандома не работает, не могу подобрать нужную функцию пути
            dMot = calcMotivator(curPos, new_short_distance);
            GameBoard newBoard(board);
            newBoard.selectCell(curPos, 1);
            newBoard.moveSelectedCell(newPos);
            AIMove *pMove = new AIMove(curPos, newPos, dMot, parent, newBoard);
            moveList.push_back(pMove);
        }
    }
    return moveList;
}

/************************************************************************************/
int32_t AIPlayer::calcMotivator(Pos newPos, int32_t score){
    int lScore = calcShortDistance(newPos, {0, 7, 0});
    return score - lScore;
}

int32_t AIPlayer::calcDemotivator(int32_t lScore, int32_t sScore){
    int32_t nScore = ((lScore - sScore)+(randrange(2)-lScore)+(randrange(10)+sScore))*((float)lScore/(float(sScore)));
    return nScore;
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
		for(size_t j = 0; j < moveList.size(); j++){
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
		for(size_t j = 0; j < moveList.size(); j++)
			getBestMove(moveList[j]->next,layer + 1, maxLayer);
		int min = INT_MAX;
		for(size_t j = 0; j < moveList.size(); j++){
			if(moveList[j]->score < min)
				min = moveList[j]->score;
		}

		if(moveList[0]->previous != NULL)
			moveList[0]->previous->score += min;
	}

	if(layer == 0){
		AIMove *retMove;
		int maxV = INT_MIN;
		for(size_t i = 0; i < moveList.size(); i++)
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