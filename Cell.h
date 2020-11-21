#ifndef __CELL__
#define __CELL__

#include "Figure.h"

class Cell{
public:
    Cell(){
        figure = new Figure();
        isCellContainFigure = false;
    }

    ~Cell(){
        delete figure;
    }

    void placeFigure(uint32_t fig_id, uint32_t owner_id) { 
        figure->init(fig_id, owner_id); 
        isCellContainFigure = true;
    }

    void placeFigure(Figure *figure) { 
        this->figure->init(figure->getId(), figure->getOwnerId());
        isCellContainFigure = true;
    }
    
    Figure *getFigure() { return figure; }

    void replaceFigure() { 
        isCellContainFigure = false;
    }

    bool isEmpty() { return !isCellContainFigure; }
private:
    Figure *figure;
    bool isCellContainFigure;
};

#endif