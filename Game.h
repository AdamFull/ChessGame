#ifndef __GAME__
#define __GAME__

#include "stdint.h"

#include "Player.h"

class Game{
public:
    Game();
    ~Game();

    Player *getCurrentPlayer() { return &players[curent_turn]; }
    Player *getPlayer(uint32_t player_id) { return &players[player_id]; }
    Player *getNextPlayer() { return curent_turn == 0 ? &players[1] : &players[0]; }

    uint32_t checkCursorCollision(uint32_t cursor_x, uint32_t cursor_y);
    uint32_t checkGlobalCollision(uint32_t cursor_x, uint32_t cursor_y);

    bool checkStepDelta(uint32_t cursor_x, uint32_t cursor_y);

    Pos *getFreePlaces();

    void setSelectedFigure(uint32_t figId) { selected_figure = figId; }
    uint32_t getSelectedFigure() { return selected_figure; }

    uint8_t getCurrentTurn() { return curent_turn; }
    void nextTern();

    void calcFiguresOnEnemyTeretory(uint32_t cursor_x, uint32_t cursor_y);

    bool endGame();

    bool comp(Pos first, Pos second) { return (first.pos_x == second.pos_x && first.pos_y == second.pos_y); }

private:
    Player players[2];
    uint8_t curent_turn;
    uint32_t all_time_turns;

    uint32_t selected_figure;

};

#endif