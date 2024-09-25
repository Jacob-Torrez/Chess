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
    void setCastle();
    bool getEnPassant();
    bool getCastle();
    bool getHasMoved();
    void enableEnPassant();
    void disableEnPassant();
    void updateHasMoved();
private:
    char type;
    char color;
    bool castle = true;
    bool enPassant = false;
    bool hasMoved = false;
};

#endif