#ifndef POSITION_H
#define POSITION_H

struct Position{
    Position(int r, int c) : row(r), col(c){}
    
    int row;
    int col;
};

bool operator==(Position a, Position b){
    bool equal = false;
    if (a.row == b.row && a.col == b.col){
        equal = true;
    }

    return equal;
}

#endif