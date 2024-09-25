#include "board.h"

void Board::printBoard(){

    for (int i = 0; i < 8; i++){
        cout << " +---+---+---+---+---+---+---+---+" << endl;
        for (int j = 0; j < 8; j++){
            if (board[i][j].getColor() == 'W'){
                cout << " | " << board[i][j].getType();
            }
            else{
                cout << " | " << static_cast<char>(tolower(board[i][j].getType()));
            }
        }
        cout << " |" << endl;
    }

    cout << " +---+---+---+---+---+---+---+---+" << endl;
}

bool Board::isClearPath(Position i, Position f){
    bool clearPath = true;
    if (i.row == f.row){ //hori
        if (i.col < f.col){ // right
            for (int j = i.col + 1; j < f.col && clearPath; j++){
                if (board[i.row][j].getType() != ' '){
                    clearPath = false;
                }
            }
        }
        else{ //left
            for (int j = i.col - 1; j > f.col && clearPath; j--){
                if(board[i.row][j].getType() != ' '){
                    clearPath = false;
                }
            }
        }
    }
    else if (i.col == f.col){ //vert
        if (i.row < f.col){ //down
            for (int j = i.row + 1; j < f.row && clearPath; j++){ 
                if (board[j][i.col].getType() != ' '){
                    clearPath = false;
                }
            }
        }
        else{ //up
            for (int j = i.row - 1; j > f.row && clearPath; j--){
                if (board[j][i.col].getType() != ' '){
                    clearPath = false;
                }
            }
        }
    }
    else{ //diag
        if (i.row < f.row && i.col < f.col){ // down-right
            for (int j = i.row + 1, k = i.col + 1; j < f.row && k < f.col && clearPath; j++, k++){
                if (board[j][k].getType() != ' '){
                    clearPath = false;
                }
            }
        }
        else if (i.row < f.row && i.col > f.col){ // down-left
            for (int j = i.row + 1, k = i.col - 1; j < f.row && k > f.col && clearPath; j++, k--){
                if (board[j][k].getType() != ' '){
                    clearPath = false;
                }
            }
        }
        else if (i.row > f.row && i.col < f.col){ //up-right
            for (int j = i.row - 1, k = i.col + 1; j > f.row && k < f.col && clearPath; j--, k++){
                if (board[j][k].getType() != ' '){
                    clearPath = false;
                }
            }
        }
        else { //up-left
            for (int j = i.row - 1, k = i.col - 1; j > f.row && k > f.col && clearPath; j--, k--){
                if (board[j][k].getType() != ' '){
                    clearPath = false;
                }
            }
        }
    }
}

bool Board::isValidMove(Position i, Position f){ //add input validation to ensure in range index
    bool validMove = true;

    if (i.row == f.row && i.col == f.col){ //checks if piece moves
        validMove = false; 
    }
    else{
        if (board[i.row][i.col].getColor() == board[f.row][f.col].getColor()){  //checks if f is same color as i
            validMove = false;
        }
        else{
            switch(board[i.row][f.row].getType()){
                case 'P': //pawn need to check for +-1 rows or +-2 rows && en Passant
                    break;
                    
                case 'R': //rook
                    if ((i.row == f.row) == (i.col == f.col)){ 
                        validMove = false;
                    }
                    else if (!isClearPath(i, f)){
                        validMove = false;
                    }
                    break;

                case 'N': //knight
                    if (!(((abs(i.row - f.row) == 1) && (abs(i.col - f.col) == 2)) || ((abs(i.row - f.row) == 2) && (abs(i.col - f.col) == 1)))){
                        validMove = false;
                    }
                    break;
                
                case 'Q': //queen
                    if ((i.row == f.row) == (i.col == f.col)){
                        validMove = false;
                    }
                    else if (abs(i.row - f.row) != abs(i.col - f.col)){
                        validMove = false;
                    }
                    break;

                case 'B': //bishop
                    if (abs(i.row - f.row) != abs(i.col - f.col)){
                        validMove = false;
                    }
                    break;

                case 'K': //king
                    break;
            }
        }
    }

    return validMove;
}

void Board::movePiece(Position i, Position f){

}