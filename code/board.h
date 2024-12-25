#pragma once

#include "piece.h"
#include "position.h"
#include <iostream>

class Board
{
public:
    Board();
    Board(const Board& other);
    ~Board();
    
    void printBoard() const;
    bool isMovePathClear(const Position& i, const Position& f) const;
    bool isValidMove(const Position& i, const Position& f) const;
    bool isKingInCheck(const Position& king) const;
    void movePiece(const Position& i, const Position& f);

    void updateTurn(){turn = (turn) ? 0 : 1;}
    bool getTurn() const {return turn;}

    void updateWhiteKing(const Position& p) {whiteKing = p;}
    void updateBlackKing(const Position& p) {blackKing = p;}
    Position getWhiteKing() const {return whiteKing;}
    Position getBlackKing() const {return blackKing;}

    Board& operator=(const Board& other);
    Piece* operator[](const Position& p) const {return board[p.row][p.col];}
    Piece*& operator[](const Position& p){return board[p.row][p.col];}

private:
    bool moveFound(const std::vector<Position>& moves, const Position& move) const;
    bool checkDirection(const Position& king, const Position& dir, const bool& kingColor, const bool& oppColor, const PieceType& type1, const PieceType& type2) const;

    Position whiteKing;
    Position blackKing;

    bool turn = Color::White; // white  = 1, black = 0

    Piece* board[8][8];
};