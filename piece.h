#ifndef PIECE_H
#define PIECE_H

#include <string>
#include <vector>
#include "position.h"
using namespace std;

class Piece {
public:
    Piece(char t = ' ', char c = 'x') : type(t), color(c){}
    char getType();
    void updateHistory(Position p);
private:
    char type;
    char color;
    vector<Position> history;
};

#endif