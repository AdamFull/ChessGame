#include "Game.h"
#include "math.h"

Game::Game(){
    players[0] = Player(true);
    players[1] = Player(false);
    curent_turn = 0;
    all_time_turns = 0;
    selected_figure = 9;
}

Game::~Game(){

}

uint32_t Game::checkCursorCollision(uint32_t cursor_x, uint32_t cursor_y){
    for(uint32_t i = 0; i < 9; i++){
        if(players[curent_turn].gerFigures()[i].cursorCollision(cursor_x, cursor_y)){
            return players[curent_turn].gerFigures()[i].getId();
        }
    }
    return 9;
}

uint32_t Game::checkGlobalCollision(uint32_t cursor_x, uint32_t cursor_y){
    for(uint32_t j = 0; j < 2; j++){
        for(uint32_t i = 0; i < 9; i++){
            if(players[j].gerFigures()[i].cursorCollision(cursor_x, cursor_y)){
                return players[j].gerFigures()[i].getId();
            }
        }
    }
    return 9;
}

bool Game::checkStepDelta(uint32_t cursor_x, uint32_t cursor_y){
    Pos selected = players[curent_turn].gerFigures()[selected_figure].getPosition();
    Pos new_position = { abs((int)selected.pos_x - (int)cursor_x), abs((int)selected.pos_y - (int)cursor_y) };
    
    if(new_position.pos_x > 1 || new_position.pos_y > 1) return false;
    return true;
}

Pos *Game::getFreePlaces(){
    static Pos nearbyFigures[4];
    Pos cfp = players[curent_turn].getFigureById(selected_figure).getPosition();

    if(checkCursorCollision(cfp.pos_x + 1, cfp.pos_y) == 9)
    nearbyFigures[0] = {cfp.pos_x + 1, cfp.pos_y};
    if(checkCursorCollision(cfp.pos_x - 1, cfp.pos_y) == 9)
    nearbyFigures[1] = {cfp.pos_x - 1, cfp.pos_y};
    if(checkCursorCollision(cfp.pos_x, cfp.pos_y + 1) == 9)
    nearbyFigures[2] = {cfp.pos_x, cfp.pos_y + 1};
    if(checkCursorCollision(cfp.pos_x, cfp.pos_y - 1) == 9)
    nearbyFigures[3] = {cfp.pos_x, cfp.pos_y - 1};

    return nearbyFigures;
}

void Game::nextTern(){
    if(curent_turn == 0) curent_turn = 1;
    else curent_turn = 0;
}

void Game::calcFiguresOnEnemyTeretory(uint32_t cursor_x, uint32_t cursor_y){
    Area enemyArea = getNextPlayer()->getArea();
    uint32_t counter = 0;
    Pos start_position;

    if(curent_turn == 0){
        start_position = enemyArea.start_pos;
        for(uint32_t j = 0; j < 3; j++){
            for(uint32_t i = 0; i < 3; i++){
                if(comp(getCurrentPlayer()->gerFigures()[counter].getPosition(), start_position)){
                    getCurrentPlayer()->gerFigures()[counter++].setIsInEnemyArea(true);
                }else{
                    getCurrentPlayer()->gerFigures()[counter++].setIsInEnemyArea(false);
                }
                start_position.pos_y--;
            }
            start_position.pos_y = enemyArea.start_pos.pos_y;
            start_position.pos_x++;
        }
    }else{
        start_position = enemyArea.start_pos;
        for(uint32_t j = 0; j < 3; j++){
            for(uint32_t i = 0; i < 3; i++){
                if(comp(getCurrentPlayer()->gerFigures()[counter].getPosition(), start_position)){
                    getCurrentPlayer()->gerFigures()[counter].setIsInEnemyArea(true);
                }else{
                    getCurrentPlayer()->gerFigures()[counter].setIsInEnemyArea(false);
                }
                start_position.pos_x--;
            }
            start_position.pos_x = enemyArea.start_pos.pos_x;
            start_position.pos_y++;
        }
    }
    getCurrentPlayer()->calcNumberOfFiguresOnEnemyArea();
}

bool Game::endGame(){

} 