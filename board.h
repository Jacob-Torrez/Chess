#ifndef BOARD_H
#define BOARD_H

#include "piece.h"
#include <iostream>
using namespace std;

class Board
{
public:
    void printBoard();
private:
    Piece board[8][8] = 
    {
    {Piece('R', 'B', 'a', '8'), Piece('N', 'B', 'b', '8'), Piece('B', 'B', 'c', '8'), Piece('Q', 'B', 'd', '8'), 
    Piece('K', 'B', 'e', '8'), Piece('B', 'B', 'f', '8'), Piece('N', 'B', 'g', '8'), Piece('R', 'B', 'h', '8')},

    {Piece('P', 'B', 'a', '7'), Piece('P', 'B', 'b', '7'), Piece('P', 'B', 'c', '7'), Piece('P', 'B', 'd', '7'), 
    Piece('P', 'B', 'e', '7'), Piece('P', 'B', 'f', '7'), Piece('P', 'B', 'g', '7'), Piece('P', 'B', 'h', '7')},

    {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
    {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
    {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
    {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},

    {Piece('P', 'W', 'a', '2'), Piece('P', 'W', 'b', '2'), Piece('P', 'W', 'c', '2'), Piece('P', 'W', 'd', '2'), 
    Piece('P', 'W', 'e', '2'), Piece('P', 'W', 'f', '2'), Piece('P', 'W', 'g', '2'), Piece('P', 'W', 'h', '2')},

    {Piece('R', 'W', 'a', '1'), Piece('N', 'W', 'b', '1'), Piece('B', 'W', 'c', '1'), Piece('Q', 'W', 'd', '1'), 
    Piece('K', 'W', 'e', '1'), Piece('B', 'W', 'f', '1'), Piece('N', 'W', 'g', '1'), Piece('R', 'W', 'h', '1')}
    };
};

#endif