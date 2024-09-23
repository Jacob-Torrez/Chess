#include "piece.h"

char Piece::getType(){
    return type;
}

char Piece::getColor(){
    return color;
}

void Piece::updateHasMoved(){
    hasMoved = true;
}