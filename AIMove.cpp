#include "AIMove.h"

AIMove::AIMove(Pos from, Pos to, int score, AIMove * parent, GameBoard gb){
	this->score = score;
	this->from = from;
	this->to = to;
	this->previous = parent;
	this->gameBoard = gb;
}