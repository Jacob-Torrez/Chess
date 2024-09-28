#include "piece.h"

char Piece::getType(){
    return type;
}

char Piece::getColor(){
    return color;
}

void Piece::updateCastle(){
    castle = false;
}

void Piece::updateEnPassant(bool b){
    enPassant = b;
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