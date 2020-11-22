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


/*******************************************************************************/
/********************************Input callbacks********************************/
/*******************************************************************************/
extern "C"
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_R && action == GLFW_PRESS){
        delete currentGame;
        currentGame = new Game();
    }else if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        delete currentGame;
        exit(0);
    }
}

/*******************************************************************************/
extern "C"
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    m_pos_x = xpos;
    m_pos_y = remap((uint32_t)ypos, 0, height, height, 0);
}

/*******************************************************************************/
extern "C"
void mouse_click_callback(GLFWwindow* window, int button, int action, int mods){
    uint32_t cell_x, cell_y;
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        cell_x = remap(m_pos_x, start_position_x, start_position_x + board_size, 0, 8);
        cell_y = remap(m_pos_y, start_position_y, start_position_y + board_size, 0, 8);

        if(cell_x > 7 || cell_x < 0 || cell_y > 7 || cell_x < 0){
            return;
        }

        if(currentGame->getGameBoard()->isCellSelected()){
            if(currentGame->getGameBoard()->moveSelectedCell(cell_x, cell_y)){
                currentGame->toggleTurn();
            }
        }else{
            if(currentGame->getGameBoard()->selectCell(cell_x, cell_y, 0)){
                printf("Selected figure: %d : %d\n", cell_x, cell_y);
            }
        }
    }else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
        if(currentGame->getGameBoard()->isCellSelected()){
            currentGame->getGameBoard()->deselectCell();
        }
    }
}

/*******************************************************************************/
/*********************************Board draw************************************/
/*******************************************************************************/
void draw_figures(){
    start_position_x = (width-board_size)/2;
    start_position_y = (height-board_size)/2;

    //Draw figures on board
    for(uint32_t y = 0; y < 8; y++){
        for(uint32_t x = 0; x < 8; x++){
            if(!currentGame->getGameBoard()->getCell(x, y)->isEmpty()){
                if(currentGame->getGameBoard()->isCellSelected()) {
                    Pos cellPos = currentGame->getGameBoard()->getSelectedCellPosition();
                    uint32_t colis[4] = {0};
                    memcpy(colis, currentGame->getCollisions(), 4*sizeof(uint32_t));

                    //Draw avaliable turns
                    if(colis[0] > 0) display->gfx.drawBitmap(start_position_x + (cellPos.px-1)*cell_size, start_position_y + cellPos.py*cell_size, peshka, 37, 37, CL_GREY());
                    if(colis[1] > 0) display->gfx.drawBitmap(start_position_x + cellPos.px*cell_size, start_position_y + (cellPos.py+1)*cell_size, peshka, 37, 37, CL_GREY());
                    if(colis[2] > 0) display->gfx.drawBitmap(start_position_x + (cellPos.px+1)*cell_size, start_position_y + cellPos.py*cell_size, peshka, 37, 37, CL_GREY());
                    if(colis[3] > 0) display->gfx.drawBitmap(start_position_x + cellPos.px*cell_size, start_position_y + (cellPos.py-1)*cell_size, peshka, 37, 37, CL_GREY());

                    //Spotlight selected figure
                    if(x == cellPos.px && y == cellPos.py) display->gfx.fillRect(start_position_x + x*cell_size, start_position_y + y*cell_size, cell_size, cell_size, CL_YELLOW());
                }

                //Draw figure
                display->gfx.drawBitmap(start_position_x + x*cell_size, start_position_y + y*cell_size, peshka, 37, 37, currentGame->getGameBoard()->getCell(x, y)->getFigure()->getOwnerId() == 0 ? CL_BLUE() : CL_RED());
            }
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
            //Draw white board cells
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

    //Draw numders
    for(uint32_t rc = 0; rc < cells_in_row; rc++){
        char itoc = '0' + rc + 1;
        display->gfx.drawChar(start_position_x_cursor - cell_size/2, start_position_y_cursor, CL_BLACK(), CL_WHITE(), 2, itoc);
        display->gfx.drawChar(start_position_x_cursor + board_size, start_position_y_cursor, CL_BLACK(), CL_WHITE(), 2, itoc);
        start_position_y_cursor += cell_size;
    }

    start_position_x_cursor = start_position_x + cell_size/3;
    start_position_y_cursor = start_position_y;

    //Draw characters
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

            currentGame->AI_Worker();

            display->clear();

            /*****************Draw loop******************/
            draw_game_field();
            draw_figures();
            /*****************Draw loop******************/
            display->display();
        }
    }

    delete display;
    return 0;
}