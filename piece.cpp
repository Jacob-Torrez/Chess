#include "piece.h"

char Piece::getType(){
    return type;
}

char Piece::getColor(){
    return color;
}

void Piece::setCastle(){
    castle = false;
}

void Piece::enableEnPassant(){
    enPassant = true;
}

void Piece::disableEnPassant(){
    enPassant = false;
}

void Piece::updateHasMoved(){
    hasMoved = true;
}

bool Piece::getEnPassant(){
    return enPassant;
}

bool Piece::getCastle(){
    return castle;
}

bool Piece::getHasMoved(){
    return hasMoved;
}