#ifndef __CELL__
#define __CELL__

#include "Figure.h"

typedef struct{
    int32_t px, py, dist;
} Pos;

class Cell{
public:
    Cell(){
        figure = Figure();
        isCellContainFigure = false;
    }

    ~Cell(){
        //delete figure;
    }

    void placeFigure(int32_t fig_id, int32_t owner_id) { 
        figure.init(fig_id, owner_id); 
        isCellContainFigure = true;
    }

    void placeFigure(Figure *figure) { 
        this->figure.init(figure->getId(), figure->getOwnerId());
        isCellContainFigure = true;
    }
    
    Figure *getFigure() { return &figure; }

    void replaceFigure() { 
        figure.init(99, 99); 
        isCellContainFigure = false;
    }

    void setCellPosition(int32_t x, int32_t y) { cellPosition = {x, y, 0}; }
    Pos getCellPosition() { return cellPosition; }

    bool isEmpty() { return !isCellContainFigure; }
private:
    Figure figure;
    Pos cellPosition;
    bool isCellContainFigure;
};

#endif