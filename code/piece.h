#pragma once
#include <string>
#include <vector>
#include "position.h"

enum class PieceType { Pawn, Knight, Bishop, Rook, King, Queen };
enum Color { Black, White };
constexpr int MAX_WIDTH = 8;
constexpr int MAX_HEIGHT = 8;
constexpr int MIN_WIDTH = 0;
constexpr int MIN_HEIGHT = 0;

class Piece {
public:
    Piece(bool c) : color(c) {}
    Piece(const Piece& other) : color(other.color) {}
    virtual Piece* clone() const = 0;
    virtual ~Piece() = default;

    virtual bool getColor() const {return color;}
    virtual PieceType getType() const = 0;

    virtual std::vector<Position> getPossibleMoves(const Position& i) const = 0;

protected:
    virtual std::vector<Position> generateMoves(const std::vector<Position>& directions, const Position& i) const;

private:
    bool color; // 1 is white, 0 is black
};

class King final : public Piece {
public:
    King(bool c) : Piece(c) {}
    King(const King& other) : Piece(other.getColor()), castle(other.castle) {}
    Piece* clone() const override {return new King(*this);}

    PieceType getType() const override {return PieceType::King;}
    void updateCastle() {castle = 0;}
    bool getCastle() const {return castle;}

    std::vector<Position> getPossibleMoves(const Position& i) const override;

private:
    bool castle = 1;
};

class Queen final : public Piece {
public:
    using Piece::Piece;
    Piece* clone() const override {return new Queen(*this);}

    PieceType getType() const override {return PieceType::Queen;}

    std::vector<Position> getPossibleMoves(const Position& i) const override;
};

class Bishop final : public Piece {
public:
    using Piece::Piece;
    Piece* clone() const override {return new Bishop(*this);}

    PieceType getType() const override {return PieceType::Bishop;}

    std::vector<Position> getPossibleMoves(const Position& i) const override;
};

class Rook final : public Piece {
public:
    Rook(bool c) : Piece(c) {}
    Rook(const Rook& other) : Piece(other.getColor()), castle(other.castle) {}
    Piece* clone() const override {return new Rook(*this);}

    PieceType getType() const override {return PieceType::Rook;}
    void updateCastle() {castle = 0;}
    bool getCastle() const {return castle;}

    std::vector<Position> getPossibleMoves(const Position& i) const override;

private:
    bool castle = 1;
};

class Pawn final : public Piece {
public:
    Pawn(bool c) : Piece(c) {}
    Pawn(const Pawn& other) : Piece(other.getColor()), isFirstMove(other.isFirstMove), enPassant(other.enPassant) {}
    Piece* clone() const override {return new Pawn(*this);}

    PieceType getType() const override {return PieceType::Pawn;}
    void updateEnPassant() {enPassant = (enPassant) ? 0 : 1;}
    bool getEnPassant() const {return enPassant;}
    void updateIsFirstMove() {isFirstMove = 0;}
    bool getIsFirstMove() const {return isFirstMove;}

    std::vector<Position> getPossibleMoves(const Position& i) const override;

private:
    bool isFirstMove = 1;
    bool enPassant = 0; 
};

class Knight final : public Piece {
public:
    using Piece::Piece;
    Piece* clone() const override {return new Knight(*this);}

    PieceType getType() const override {return PieceType::Knight;}

    std::vector<Position> getPossibleMoves(const Position& i) const override;
};

extern char to_char(const PieceType& piece);

extern bool isWithinBounds(const Position& p);