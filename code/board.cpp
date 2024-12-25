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
        for (int j = 0; j < MAX_HEIGHT; j++){
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

    if (i.col != f.col && i.row != f.row && (f.row - i.row != f.col - i.col)){ // makes sure path is hori, vert, or diag
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

bool Board::isValidMove(const Position& i, const Position& f) const {
    std::vector<Position> moves = (*this)[i]->getPossibleMoves(i);
    Position king = (getTurn()) ? getWhiteKing() : getBlackKing();

    if (moveFound(moves, f)){ // checks if move matches behavior of piece
        if (!(*this)[f] || (*this)[f]->getColor() != (*this)[i]->getColor()){ // checks for empty space or opposite color piece at final position
            if (isMovePathClear(i, f) || (*this)[i]->getType() == PieceType::Knight){ // checks if path is clear or if piece is knight
                Board simulatedMove(*this);
                simulatedMove.movePiece(i, f);

                if ((*this)[i]->getType() == PieceType::King){
                    king = f;
                }

                if (!simulatedMove.isKingInCheck(king)){
                    switch ((*this)[i]->getType()){
                        case PieceType::Pawn: // en passant AND first move

                            break;

                        case PieceType::King:
                            if (abs(f.col - i.col) == 2){ // checks for castle
                                int castleSide = (f.col - i.col == 2) ? 1 : -2; // determines king/queen side castle

                                simulatedMove.movePiece(f, {i.row, i.col + ((castleSide == 1) ? 1 : -1)});
                                king = {i.row, i.col + ((castleSide == 1) ? 1 : -1)};
                                if (!simulatedMove.isKingInCheck(king)){

                                    Position rook = {i.row, f.col + castleSide};
                                    if ((*this)[rook] && (*this)[rook]->getType() == PieceType::Rook && dynamic_cast<Rook*>((*this)[rook])->getCastle()){ // checks for rook and if can castle
                                        if (castleSide == -2){ // if queen side check path
                                            if (isMovePathClear(rook, {i.row, f.col + 1})){
                                                return true;
                                            }
                                        }
                                        else {
                                            return true;
                                        }
                                    }
                                }
                            }
                            else {
                                return true;
                            }
                            break;

                        default:
                            return true;
                    }
                }
            }
        }
    }

    return false;
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