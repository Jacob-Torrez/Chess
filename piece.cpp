#include "piece.h"

char Piece::getType(){
    char temp;
    if (color == 'W'){
        temp = type;
    }
    else{
        temp = tolower(type);
    }

    return temp;
}