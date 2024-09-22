#include "board.h"

void Board::printBoard(){

    for (int i = 0; i < 8; i++){
        cout << " +---+---+---+---+---+---+---+---+" << endl;
        for (int j = 0; j < 8; j++){
            cout << " | " << board[i][j].getType();
        }
        cout << " |" << endl;
    }
}

bool Board::validMove(Position i, Position f){
    
}

void Board::movePiece(Position i, Position f){
    
}