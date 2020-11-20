#include "Player.h"

Player::Player(){

}

Player::Player(bool player_number){
    uint32_t counter = 0;
    Pos start_position;
    this->player_number = player_number;

    figures_in_enemy_area = 0;

    if(player_number){
        //down right angle
        player_zone_area = { {7, 0}, {5, 2} };
        start_position = player_zone_area.start_pos;
        for(uint32_t j = 0; j < 3; j++){
            for(uint32_t i = 0; i < 3; i++){
                figures[counter].setId(counter);
                figures[counter++].setFigurePosition(start_position.pos_x, start_position.pos_y);
                start_position.pos_x--;
            }
            start_position.pos_x = player_zone_area.start_pos.pos_x;
            start_position.pos_y++;
        }
    }else{
        //top left angle
        player_zone_area = { {0, 7}, {2, 5} };
        start_position = player_zone_area.start_pos;
        for(uint32_t j = 0; j < 3; j++){
            for(uint32_t i = 0; i < 3; i++){
                figures[counter].setId(counter);
                figures[counter++].setFigurePosition(start_position.pos_x, start_position.pos_y);
                start_position.pos_y--;
            }
            start_position.pos_y = player_zone_area.start_pos.pos_y;
            start_position.pos_x++;
        }
    }
}

 Player::~Player(){

 }

void Player::move_figure(uint8_t figure_number, Pos new_position){
    figures[figure_number].setFigurePosition(new_position);
}

Figure Player::getFigureById(uint32_t id){
    for(uint32_t i = 0; i < 9; i++){
        if(figures[i].getId() == id) return figures[i];
    }
}

void Player::calcNumberOfFiguresOnEnemyArea(){
    figures_in_enemy_area = 0;
    for(uint32_t i = 0; i < 9; i++){
        if(figures[i].getIsInEnemyArea()){
            figures_in_enemy_area++;
        }
    }
}