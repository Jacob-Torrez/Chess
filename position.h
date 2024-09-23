#ifndef POSITION_H
#define POSITION_H

struct Position{
    Position(int r, int c) : row(r), col(c){}
    
    int row;
    int col;
};

#endif