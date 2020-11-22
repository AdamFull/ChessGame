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

    void placeFigure(uint32_t fig_id, uint32_t owner_id) { 
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

    void setCellPosition(uint32_t x, uint32_t y) { cellPosition = {x, y}; }
    Pos getCellPosition() { return cellPosition; }

    bool isEmpty() { return !isCellContainFigure; }
private:
    Figure figure;
    Pos cellPosition;
    bool isCellContainFigure;
};

#endif