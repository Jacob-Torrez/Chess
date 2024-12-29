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
            if (board[i][j]){
                delete board[i][j];
                board[i][j] = nullptr;
            }
        }
    }
}

Board::Board(const Board& other) {
    for (int i = 0; i < MAX_HEIGHT; i++){
        for (int j = 0; j < MAX_WIDTH; j++){
            if (other.board[i][j]){
                board[i][j] = other.board[i][j]->clone();
            }
            else {
                board[i][j] = nullptr;
            }
        }
    }
}

Board& Board::operator=(const Board& other){
    if (this == &other){
        return *this;
    }

    for (int i = 0; i < MAX_HEIGHT; i++){
        for (int j = 0; j < MAX_WIDTH; j++){
            delete board[i][j];
            if (other.board[i][j]){
                board[i][j] = other.board[i][j]->clone();
            }
            else {
                board[i][j] = nullptr;
            }
        }
    }

    return *this;
}

bool Board::moveFound(const std::vector<Position>& moves, const Position& move) const {
    for (const auto& m : moves){
        if (m == move){
            return true;
        }
    }

    return false;
}

bool Board::checkDirection(const Position& king, const Position& dir, const bool& kingColor, const bool& oppColor, const PieceType& type1, const PieceType& type2) const {
    Position current = king + dir;
        while (isWithinBounds(current)){
            if ((*this)[current]){
                if ((*this)[current]->getColor() == kingColor){
                    break;
                }
                else if ((*this)[current]->getColor() == oppColor && ((*this)[current]->getType() == type1 || (*this)[current]->getType() == type2)){
                    return true;
                }
            }

            current += dir;
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

bool Board::isMovePathClear(const Position& i, const Position& f) const {
    int rowStep = (f.row > i.row) - (f.row < i.row); // gets direction as 0, 1, or -1
    int colStep = (f.col > i.col) - (f.col < i.col);

    Position step(rowStep, colStep);

    if (i.col != f.col && i.row != f.row && abs(f.row - i.row) != abs(f.col - i.col)){ // makes sure path is hori, vert, or diag
        return false;
    }

    Position current = i + step;
    while (current != f){
        if ((*this)[current]){
            return false;
        }
        current += step;
    }

    return true;
}

bool Board::kingMoveValidation(const Position& i, const Position& f, Board& simulatedBoard) const {
    if (abs(f.col - i.col) == 2){ // checks for castle

        int castleDirection = (f.col > i.col) ? 1 : -1;

        // determine if rook is available to castle
        Position rookPosition = (castleDirection == 1) ? Position{i.row, MAX_HEIGHT - 1} : Position{i.row, 0};
        if (!(*this)[rookPosition] || (*this)[rookPosition]->getType() != PieceType::Rook || !dynamic_cast<Rook*>((*this)[rookPosition])->getCastle()) {
            return false;
        }

        // make sure king isn't castling through check
        Position intermediateKingPosition = {i.row, i.col + castleDirection};
        simulatedBoard.movePiece(f, intermediateKingPosition);
        if (simulatedBoard.isKingInCheck(intermediateKingPosition)) {
            return false;
        }

        // check rook path for queen side castle
        if (castleDirection == -1 && !isMovePathClear(rookPosition, {i.row, i.col - 1})) {
            return false;
        }
    }
    
    return true;
}

bool Board::pawnMoveValidation(const Position& i, const Position& f) const {
    Position direction = {f.row - i.row, f.col - i.col};

    // check for diagonal move
    if (abs(direction.col) == 1 && abs(direction.row) == 1){
        if ((*this)[f] && (*this)[f]->getColor() != (*this)[i]->getColor()){
            return true; // standard capture
        }

        if ((*this)[{i.row, f.col}] && (*this)[{i.row, f.col}]->getType() == PieceType::Pawn &&
            (*this)[{i.row, f.col}]->getColor() != (*this)[i]->getColor() &&
            dynamic_cast<Pawn*>((*this)[{i.row, f.col}])->getEnPassant()){
                return true; // en passant
        }
    }

    if (abs(direction.row) == 2 && direction.col == 0){
        if (dynamic_cast<Pawn*>((*this)[i])->getIsFirstMove() && !(*this)[f]){
            return true;
        }
    }

    if (abs(direction.row) == 1 && direction.col == 0) {
        if (!(*this)[f]){
            return true;
        }
    }

    return false;
}

bool Board::isValidMove(const Position& i, const Position& f) const {
    std::vector<Position> possibleMoves = (*this)[i]->getPossibleMoves(i);
    Position kingPosition = (getTurn()) ? getWhiteKing() : getBlackKing();

    if (!moveFound(possibleMoves, f)) {return false;} // checks if move matches behavior of piece

    if ((*this)[f] && (*this)[f]->getColor() == (*this)[i]->getColor()) {return false;} // checks for empty space or opposite color

    if (!isMovePathClear(i, f) && (*this)[i]->getType() != PieceType::Knight) {return false;} // checks if path is clear or knight

    // if king is moving update its position
    if ((*this)[i]->getType() == PieceType::King){
        kingPosition = f;
    }

    // check if new position leaves king in check
    Board simulatedBoard(*this);
    simulatedBoard.movePiece(i, f);
    if (simulatedBoard.isKingInCheck(kingPosition)){return false;}

    // final checks for castle and en passant
    switch ((*this)[i]->getType()){
        case PieceType::Pawn: 
            return pawnMoveValidation(i, f);

        case PieceType::King:
            return kingMoveValidation(i, f, simulatedBoard);

        default:
            return true;
    }
}

bool Board::isKingInCheck(const Position& king) const {
    bool kingColor = (*this)[king]->getColor();
    bool oppColor = !kingColor;

    // pawn checks
    int pawnAttackDirection = (kingColor) ? -1 : 1;
    Position pawnAttackDirections[] = {{pawnAttackDirection * 1, 1}, {pawnAttackDirection * 1, -1}};
    for (const auto& dir : pawnAttackDirections){
        Position pawn = king + dir;
        if (isWithinBounds(pawn) && (*this)[pawn] && (*this)[pawn]->getType() == PieceType::Pawn && (*this)[pawn]->getColor() == oppColor){
            return true;
        }
    }

    // knight checks
    Position knightAttackDirections[] = {{1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};
    for (const auto& dir : knightAttackDirections){
        Position knight = king + dir;
        if (isWithinBounds(knight) && (*this)[knight] && (*this)[knight]->getType() == PieceType::Knight && (*this)[knight]->getColor() == oppColor){
            return true;
        }
    }

    // bishop/queen checks
    std::vector<Position> diagonalAttackDirections = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
    for (const auto& dir : diagonalAttackDirections){
        if (checkDirection(king, dir, kingColor, oppColor, PieceType::Bishop, PieceType::Queen)){
            return true;
        }
    }

    // queen/rook checks
    std::vector<Position> orthogonalAttackDirections = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    for (const auto& dir : orthogonalAttackDirections){
        if (checkDirection(king, dir, kingColor, oppColor, PieceType::Rook, PieceType::Queen)){
            return true;
        }
    }

    return false;
}

void Board::movePiece(const Position& i, const Position& f) {
    delete (*this)[f];
    (*this)[f] = (*this)[i];
    (*this)[i] = nullptr;
}