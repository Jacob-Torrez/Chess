#ifndef PIECE_H
#define PIECE_H

#include <string>
#include <vector>
using namespace std;

struct Position{
    Position(char f, int r) : file(f), rank(r){}
    char file;
    char rank;
};

class Piece {
public:
    Piece(char t = ' ', char c = 'x', char f = '-', char r = '-') : type(t), color(c), position(f,r){}
    char getType();
    void setPosition();
private:
    char type;
    char color;
    Position position;
    vector<Position> history;
};

#endif