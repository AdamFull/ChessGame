#ifndef __FIGURE__
#define __FIGURE__

#include "stdint.h"

class Figure{
public:
    Figure(){
        fig_id = 99;
        owner_id = 99;
    }

    void init(uint32_t fig_id, uint32_t owner_id){
        this->fig_id = fig_id;
        this->owner_id = owner_id;
    }

    uint32_t getId() { return fig_id; }
    uint32_t getOwnerId() { return owner_id; }

private:
    uint32_t fig_id;
    uint32_t owner_id;
};

#endif