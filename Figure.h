#ifndef __FIGURE__
#define __FIGURE__

#include "stdint.h"

//Position struct for figures
typedef struct {
    uint8_t pos_x, pos_y;
} Pos;

//Game logic structure for detecting player area
typedef struct {
    Pos start_pos, end_pos;
} Area;

class Figure{
public:
    Figure();
    ~Figure();

    void setFigurePosition(uint8_t x, uint8_t y) { fig_pos = {x, y}; }
    void setFigurePosition(Pos new_pos) { fig_pos = new_pos; }
    void setOwner(bool new_owner) { owner = new_owner; }

    void setId(uint32_t id) { fig_id = id; }
    uint32_t getId() { return fig_id; }

    bool cursorCollision(uint8_t x, uint8_t y) { return (x == fig_pos.pos_x && y == fig_pos.pos_y); }

    void setIsInEnemyArea(bool state) { is_in_enemy_area = state; }
    bool getIsInEnemyArea() { return is_in_enemy_area; }

    Pos getPosition() { return fig_pos; }

private:
    Pos fig_pos;
    uint32_t fig_id;
    bool is_in_enemy_area;
    bool owner;
};

#endif