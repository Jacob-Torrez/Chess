#pragma once
#include <string>
#include <vector>
#include "position.h"

enum class PieceType { Pawn, Knight, Bishop, Rook, King, Queen };
enum Color { Black, White };
const int MAX_WIDTH = 8;
const int MAX_HEIGHT = 8;
const int MIN_WIDTH = 0;
const int MIN_HEIGHT = 0;

class Piece {
public:
    Piece(bool c) : color(c) {}
    virtual ~Piece() = default;
    bool getColor() {return color;}
    virtual PieceType getType() = 0;
    virtual std::vector<Position> getPossibleMoves(const Position& i) = 0;

protected:
    bool isWithinBounds(const Position& p);
    std::vector<Position> checkDirections(const std::vector<Position>& directions, const Position& i);

private:
    bool color; // 1 is white, 0 is black
};

class King : public Piece {
public:
    King(bool c) : Piece(c) {}
    PieceType getType() override {return PieceType::King;}
    std::vector<Position> getPossibleMoves(const Position& i) override;
    void updateCastle() {castle = 0;}
    bool getCastle() {return castle;}

private:
    bool castle = 1;
};

class Queen : public Piece {
public:
    Queen(bool c) : Piece(c) {}
    PieceType getType() override {return PieceType::Queen;}
    std::vector<Position> getPossibleMoves (const Position& i) override;
};

class Bishop : public Piece {
public:
    Bishop(bool c) : Piece(c) {}
    PieceType getType() override {return PieceType::Bishop;}
    std::vector<Position> getPossibleMoves(const Position& i) override;
};

class Rook : public Piece {
public:
    Rook(bool c) : Piece(c) {}
    PieceType getType() override {return PieceType::Rook;}
    std::vector<Position> getPossibleMoves(const Position& i) override;
    void updateCastle() {castle = 0;}
    bool getCastle() {return castle;}

private:
    bool castle = 1;
};

class Pawn : public Piece {
public:
    Pawn(bool c) : Piece(c) {}
    PieceType getType() override {return PieceType::Pawn;}
    std::vector<Position> getPossibleMoves(const Position& i) override;
    void updateEnPassant() {enPassant = (enPassant) ? 0 : 1;}
    bool getEnPassant() {return enPassant;}
    void updateIsFirstMove() {isFirstMove = 0;}
    bool getIsFirstMove() {return isFirstMove;}

private:
    bool isFirstMove = 1;
    bool enPassant = 0; 
};

class Knight : public Piece {
public:
    Knight(bool c) : Piece(c) {}
    PieceType getType() override {return PieceType::Knight;}
    std::vector<Position> getPossibleMoves(const Position& i) override;
};

extern char to_char(const PieceType& piece);