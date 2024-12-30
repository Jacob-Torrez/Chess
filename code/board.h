#pragma once

#include "piece.h"
#include "position.h"
#include <iostream>

class Board {
public:
    Board();
    Board(const Board& other);
    ~Board();
    
    void printBoard() const;
    bool isValidMove(const Position& i, const Position& f, const bool& turn) const;
    bool isKingInCheck(const Position& king) const;
    void movePiece(const Position& i, const Position& f, const bool& turn);

    void updateWhiteKing(const Position& p) {whiteKing = p;}
    void updateBlackKing(const Position& p) {blackKing = p;}
    Position getWhiteKing() const {return whiteKing;}
    Position getBlackKing() const {return blackKing;}

    Board& operator=(const Board& other);

private:
    Piece* getPiece(const Position& p) const {return board[p.row][p.col];}
    Piece*& getPiece(const Position& p) {return board[p.row][p.col];}
    bool isMovePathClear(const Position& i, const Position& f) const;
    bool moveFound(const std::vector<Position>& moves, const Position& move) const;
    bool checkDirection(const Position& king, const Position& dir, const bool& kingColor, const bool& oppColor, const PieceType& type1, const PieceType& type2) const;
    bool kingMoveValidation(const Position& i, const Position& f, Board& simulatedBoard) const;
    bool pawnMoveValidation(const Position& i, const Position& f) const;

    Position whiteKing = {7,4};
    Position blackKing = {0,4};

    Piece* board[8][8];
};