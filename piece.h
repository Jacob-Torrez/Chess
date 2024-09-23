#ifndef PIECE_H
#define PIECE_H

#include <string>
#include "position.h"
using namespace std;

class Piece {
public:
    Piece(char t = ' ', char c = 'x') : type(t), color(c){}
    char getType();
    char getColor();
    void updateHasMoved();
private:
    char type;
    char color;
    bool hasMoved = false;
};

#endif