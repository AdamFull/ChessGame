#ifndef __PLAYER__
#define __PLAYER__

#include "Figure.h"

class Player{
public:
    Player();
    Player(bool player_number);
    ~Player();

    void move_figure(uint8_t figure_number, Pos new_position);

    Figure *gerFigures() { return figures; }
    bool getPlayerNumber() { return player_number; }
    Area getArea() { return player_zone_area; }

    Figure getFigureById(uint32_t id);

    void calcNumberOfFiguresOnEnemyArea();

    uint32_t getNumberOfFiguresOnEnemyArea() { return figures_in_enemy_area; }

private:
    Figure figures[9];
    bool player_number;
    Area player_zone_area;

    uint32_t figures_in_enemy_area;

};

#endif