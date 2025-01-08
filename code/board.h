#pragma once

#include "piece.h"
#include "position.h"
#include <iostream>

struct MoveInfo {
    Position start;
    Position end;
    Piece* movedPiece;
    Piece* capturedPiece;
    Position capturedPositionStart;
    Position capturedPositionEnd;
    bool castlingMove;
    bool enPassantMove;
    std::vector<Position> enPassantUpdates;
};

class Board {
public:
    Board();
    Board(const Board& other);
    ~Board();
    Board& operator=(const Board& other);
    
    void printBoard() const;

    bool isValidMove(const Position& i, const Position& f, const bool& turn) const;
    bool isCheckmate(const bool& turn) const;
    bool isDraw(const bool& turn) const;

    MoveInfo movePiece(const Position& i, const Position& f, const bool& turn);
    void undoMove(MoveInfo& moveInfo);

    std::vector<std::pair<Position, Position>> getLegalMoves(const bool& turn) const;

    Piece* getPiece(const Position& p) const {return board[p.row][p.col];}

private:
    void disableEnPassant(MoveInfo& moveInfo);
    void revertEnPassant(const MoveInfo& moveInfo);
    Piece*& getPiece(const Position& p) {return board[p.row][p.col];}
    bool isKingInCheck(const Position& king) const;
    void updateSpecialRules(const Position& i, const Position& f, MoveInfo& moveInfo);
    bool checkMovePath(const Position& i, const Position& f) const;
    bool possibleMoveFound(const std::vector<Position>& moves, const Position& move) const;
    bool checkAttackDirection(const Position& king, const Position& dir, const bool& kingColor, const bool& oppColor, const PieceType& type1, const PieceType& type2) const;
    bool kingMoveValidation(const Position& i, const Position& f, Board& simulatedBoard) const;
    bool pawnMoveValidation(const Position& i, const Position& f) const;
    std::pair<int, int> countMaterial() const;

    Position whiteKing;
    Position blackKing;

    Piece* board[8][8];

};