#ifndef BOARD_H
#define BOARD_H

#include "piece.h"
#include <iostream>
using namespace std;

class Board
{
public:
    void printBoard();
    bool isValidMove(Position i, Position f);
    bool isClearPath(Position i, Position f);
    void movePiece(Position i, Position f);
private:
    Piece board[8][8] = 
    {
    {Piece('R', 'B'), Piece('N', 'B'), Piece('B', 'B'), Piece('Q', 'B'), 
    Piece('K', 'B'), Piece('B', 'B'), Piece('N', 'B'), Piece('R', 'B')},

    {Piece('P', 'B'), Piece('P', 'B'), Piece('P', 'B'), Piece('P', 'B'), 
    Piece('P', 'B'), Piece('P', 'B'), Piece('P', 'B'), Piece('P', 'B')},

    {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
    {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
    {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
    {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},

    {Piece('P', 'W'), Piece('P', 'W'), Piece('P', 'W'), Piece('P', 'W'), 
    Piece('P', 'W'), Piece('P', 'W'), Piece('P', 'W'), Piece('P', 'W')},

    {Piece('R', 'W'), Piece('N', 'W'), Piece('B', 'W'), Piece('Q', 'W'), 
    Piece('K', 'W'), Piece('B', 'W'), Piece('N', 'W'), Piece('R', 'W')}
    };
};

#endif