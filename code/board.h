#pragma once

#include "piece.h"
#include "position.h"
#include <iostream>
using namespace std;

class Board
{
public:
    Board();
    ~Board();
    void printBoard() const;
    bool isPathClear(const Position& i, const Position& f) const;
    bool isValidMove(const Position& i, const Position& f) const;
    bool kingCheck(const Position& king) const;
    void movePiece(const Position& i, const Position& f);

    Piece* operator[](const Position& p) const {return board[p.row][p.col];}
    Piece*& operator[](const Position& p){return board[p.row][p.col];}

private:
    Position whiteKing;
    Position blackKing;
    Piece* board[8][8];
};