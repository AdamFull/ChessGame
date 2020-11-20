#include "SE_MAIN.h"
#include "string"

#include "Game.h"

uint32_t width = 800;
uint32_t height = 600;

uint32_t board_size = 300;
uint32_t m_pos_x, m_pos_y;

#define cells_in_row 8

uint32_t cell_size;

uint32_t start_position_x, start_position_y;
uint32_t start_position_x_cursor, start_position_y_cursor;

SE_MAIN *display;

Game *currentGame;

static const unsigned char peshka[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x7e, 0xfe, 0xfe, 0xfe, 0xfe, 
    0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0x7e, 0x3e, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x07, 0x0f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x0f, 0x03, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xe0, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x03, 0x03, 0x07, 0x07, 0x07, 0x07, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

long remap(long x, long in_min, long in_max, long out_min, long out_max){
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


//Input callbacks
extern "C"
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    
}

extern "C"
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    m_pos_x = xpos;
    m_pos_y = remap((uint32_t)ypos, 0, height, height, 0);
}

extern "C"
void mouse_click_callback(GLFWwindow* window, int button, int action, int mods){
    uint32_t cell_x, cell_y;
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        cell_x = remap(m_pos_x, start_position_x, start_position_x + board_size, 0, 8);
        cell_y = remap(m_pos_y, start_position_y, start_position_y + board_size, 0, 8);

        if(cell_x > 7 || cell_x < 0 || cell_y > 7 || cell_x < 0){
            return;
        }

        if(currentGame->getSelectedFigure() != 9){
            if(currentGame->checkGlobalCollision(cell_x, cell_y) == 9 && currentGame->checkStepDelta(cell_x, cell_y)){
                currentGame->getCurrentPlayer()->move_figure(currentGame->getSelectedFigure(), {cell_x, cell_y});
                currentGame->setSelectedFigure(9);
                currentGame->calcFiguresOnEnemyTeretory(cell_x, cell_y);
                currentGame->nextTern();
            }
        }else{
            //Checking is figure exist on this position
            uint32_t collision_result = currentGame->checkCursorCollision(cell_x, cell_y);
            if(collision_result > 8) return;

            currentGame->setSelectedFigure(collision_result);
            printf("Figure %d: %d : %d\n", collision_result, cell_x, cell_y);
        }
    }else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
        if(currentGame->getSelectedFigure() != 9){
            currentGame->setSelectedFigure(9);
        }
    }
}

/*******************************************************************************/
void draw_figures(){
    start_position_x = (width-board_size)/2;
    start_position_y = (height-board_size)/2;

    for(uint32_t n = 0; n < 2; n++){
        start_position_x_cursor = start_position_x;
        start_position_y_cursor = start_position_y;

        for(uint32_t i = 0; i < 9; i++){
            Pos current_position = currentGame->getPlayer(n)->gerFigures()[i].getPosition();
            start_position_x_cursor = start_position_x + cell_size * current_position.pos_x;
            start_position_y_cursor = start_position_y + cell_size * current_position.pos_y;

            if(currentGame->getCurrentTurn() == n && currentGame->getSelectedFigure() == i){
                display->gfx.fillRect(start_position_x_cursor, start_position_y_cursor, cell_size, cell_size, CL_YELLOW());

                Pos nearbyPositions[4];
                for(uint32_t k = 0; k < 4; k++) nearbyPositions[k] = currentGame->getFreePlaces()[k];
                //memcpy(nearbyPositions ,, sizeof(Pos) * 4);
                for(uint32_t j = 0; j < 4; j++){
                    display->gfx.fillRect(start_position_x + (nearbyPositions[i].pos_x*cell_size), start_position_y + (nearbyPositions[i].pos_y*cell_size), cell_size, cell_size, CL_YELLOW());
                }
            }

            display->gfx.drawBitmap(start_position_x_cursor, start_position_y_cursor, peshka, 37, 37, n == 0 ? CL_BLUE() : CL_RED());
            
        }
    }
}

/*******************************************************************************/
void draw_game_field(){
    bool is_white_cell = false;
    start_position_x = (width-board_size)/2;
    start_position_y = (height-board_size)/2;
    start_position_x_cursor = start_position_x;
    start_position_y_cursor = start_position_y;

    //draw board outline
    display->gfx.drawRect(start_position_x-1, start_position_y-1, board_size-2, board_size-2, CL_YELLOW());

    for(uint32_t c = 0; c < cells_in_row; c++){
        for(uint32_t r = 0; r < cells_in_row; r++){
            if(is_white_cell){
                display->gfx.fillRect(start_position_x_cursor, start_position_y_cursor, cell_size, cell_size, CL_WHITE());
            }

            //Mouse cursor displaying
            if(m_pos_x > start_position_x_cursor  && m_pos_x < start_position_x_cursor + cell_size){
                if(m_pos_y > start_position_y_cursor  && m_pos_y < start_position_y_cursor + cell_size){
                    display->gfx.fillRect(start_position_x_cursor, start_position_y_cursor, cell_size, cell_size, CL_GREY());
                }
            }

            is_white_cell = !is_white_cell;
            start_position_x_cursor += cell_size;
        }
        is_white_cell = !is_white_cell;
        start_position_x_cursor = start_position_x;
        start_position_y_cursor += cell_size;
    }

    start_position_x_cursor = start_position_x;
    start_position_y_cursor = start_position_y + cell_size/1.5f;

    for(uint32_t rc = 0; rc < cells_in_row; rc++){
        char itoc = '0' + rc + 1;
        display->gfx.drawChar(start_position_x_cursor - cell_size/2, start_position_y_cursor, CL_BLACK(), CL_WHITE(), 2, itoc);
        display->gfx.drawChar(start_position_x_cursor + board_size, start_position_y_cursor, CL_BLACK(), CL_WHITE(), 2, itoc);
        start_position_y_cursor += cell_size;
    }

    start_position_x_cursor = start_position_x + cell_size/3;
    start_position_y_cursor = start_position_y;

    for(uint32_t rc = 0; rc < cells_in_row; rc++){
        char itoc = 'a' + rc;
        display->gfx.drawChar(start_position_x_cursor, start_position_y_cursor - cell_size/3, CL_BLACK(), CL_WHITE(), 2, itoc);
        display->gfx.drawChar(start_position_x_cursor, start_position_y_cursor + board_size + cell_size/3, CL_BLACK(), CL_WHITE(), 2, itoc);
        start_position_x_cursor += cell_size;
    }
}

/*******************************************************************************/
int main(int argc, char* argv[]){
    char tempBuffer[64] = {0};
    display = new SE_MAIN(width, height, key_callback, mouse_callback, mouse_click_callback);

    cell_size = board_size/cells_in_row;

    currentGame = new Game();

    if(display->init("..\\IoT_Screen_Emulator\\shaders\\")){
        while(display->getWindowWorker()){
            display->clear();
            /*****************Draw loop******************/
            display->gfx.setTextSize(2);
            display->gfx.setCursor(0, height-10);
            display->gfx.print("Score board");
            display->gfx.setCursor(0, height-30);
            if(currentGame->getCurrentTurn() == 1) display->gfx.print(">");
            display->gfx.print("Red: ", CL_RED());
            itoa(currentGame->getPlayer(1)->getNumberOfFiguresOnEnemyArea(), tempBuffer, 10);
            display->gfx.print(tempBuffer, CL_RED());
            display->gfx.setCursor(0, height-50);
            itoa(currentGame->getPlayer(0)->getNumberOfFiguresOnEnemyArea(), tempBuffer, 10);
            if(currentGame->getCurrentTurn() == 0) display->gfx.print(">");
            display->gfx.print("Blue: ", CL_BLUE());
            display->gfx.println(tempBuffer, CL_BLUE());
            draw_game_field();
            draw_figures();
            /*****************Draw loop******************/
            display->display();
        }
    }

    delete display;
    return 0;
}