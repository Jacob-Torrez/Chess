#include "board.h"

Board::Board(){
    for (int i = 0; i < MAX_HEIGHT; i++){
        for (int j = 0; j < MAX_WIDTH; j++){
            board[i][j] = nullptr;
        }
    }
}

Board::~Board(){
    for (int i = 0; i < MAX_HEIGHT; i++){
        for (int j = 0; j < MAX_WIDTH; j++){
            delete board[i][j];
        }
    }
}

bool Board::moveFound(const std::vector<Position>& moves, const Position& move){
    for (const auto& m : moves){
        if (m == move){
            return true;
        }
    }

    return false;
}

void Board::printBoard() const {
    for (int i = 0; i < MAX_HEIGHT; i++){
        std::cout << " +---+---+---+---+---+---+---+---+" << std::endl;
        for (int j = 0; j < MAX_WIDTH; j++){
            if (board[i][j]){
                if (board[i][j]->getColor() == Color::White){
                    std::cout << " | " << to_char(board[i][j]->getType());
                }
                else{
                    std::cout << " | " << static_cast<char>(tolower(to_char(board[i][j]->getType())));
                }
            }
            else {
                std::cout << " | " << " ";
            }
        }
        std::cout << " |" << std::endl;
    }

    std::cout << " +---+---+---+---+---+---+---+---+" << std::endl;
}

bool Board::isPathClear(const Position& i, const Position& f) const {
    int rowStep = (f.row > i.row) - (f.row < i.row);
    int colStep = (f.col > i.col) - (f.col < i.col);

    Position step(rowStep, colStep);
    Position current = i + step;

    if (i.col != f.col && i.row != f.row && (f.row - i.row != f.col - i.col)){
        return false;
    }

    while (current != f){
        if ((*this)[current]){
            return false;
        }
        current += step;
    }

    return true;
}

bool Board::isValidMove(const Position& i, const Position& f) const {
    std::vector<Position> moves = (*this)[i]->getPossibleMoves(i);

    if (moveFound(moves, f)){
        if (true){ // king in check?
            if ((*this)[i]->getType() == PieceType::Pawn){ // pawn en passant checking
        

            }
            else if ((*this)[i]->getType() == PieceType::King){ // king castling checking


            }
            else {


            }
        }
    }

    return false;
}

bool Board::kingCheck(const Position& king) const {
    bool kingColor = (*this)[king]->getColor();
    bool oppColor = !kingColor;

    

    return true;
}

void Board::movePiece(const Position& i, const Position& f){

}