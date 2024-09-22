#ifndef POSITION_H
#define POSITION_H

struct Position{
    Position(int f, int r) : file(f), rank(r){}
    
    int file;
    int rank;
};

#endif