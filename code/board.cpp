#include "board.h"

Board::Board(){
    for (int i = 0; i < MAX_HEIGHT; i++){
        for (int j = 0; j < MAX_WIDTH; j++){
            board[i][j] = nullptr;
        }
    }

    // pawns
    for (int j = 0; j < MAX_WIDTH; j++){
        board[1][j] = new Pawn(Color::Black);
        board[6][j] = new Pawn(Color::White);
    }

    // rooks
    board[0][0] = new Rook(Color::Black);
    board[0][7] = new Rook(Color::Black);
    board[7][0] = new Rook(Color::White);
    board[7][7] = new Rook(Color::White);

    // knights
    board[0][1] = new Knight(Color::Black);
    board[0][6] = new Knight(Color::Black);
    board[7][1] = new Knight(Color::White);
    board[7][6] = new Knight(Color::White);

    // bishops
    board[0][2] = new Bishop(Color::Black);
    board[0][5] = new Bishop(Color::Black);
    board[7][2] = new Bishop(Color::White);
    board[7][5] = new Bishop(Color::White);

    // queens
    board[0][3] = new Queen(Color::Black);
    board[7][3] = new Queen(Color::White);

    // kings
    board[0][4] = new King(Color::Black);
    board[7][4] = new King(Color::White);
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
            if (getPiece(current)){
                if (getPiece(current)->getColor() == kingColor){
                    break;
                }
                else if (getPiece(current)->getColor() == oppColor && (getPiece(current)->getType() == type1 || getPiece(current)->getType() == type2)){
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
        if (getPiece(current)){
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
        if (!getPiece(rookPosition) || getPiece(rookPosition)->getType() != PieceType::Rook || !dynamic_cast<Rook*>(getPiece(rookPosition))->getCastle()) {
            return false;
        }

        // make sure king isn't castling through check
        Position intermediateKingPosition = {i.row, i.col + castleDirection};
        delete simulatedBoard.getPiece(intermediateKingPosition);
        simulatedBoard.getPiece(intermediateKingPosition) = simulatedBoard.getPiece(f);
        simulatedBoard.getPiece(f) = nullptr;
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
        if (getPiece(f) && getPiece(f)->getColor() != getPiece(i)->getColor()){
            return true; // standard capture
        }

        if (getPiece({i.row, f.col}) && getPiece({i.row, f.col})->getType() == PieceType::Pawn &&
            getPiece({i.row, f.col})->getColor() != getPiece(i)->getColor() &&
            dynamic_cast<Pawn*>(getPiece({i.row, f.col}))->getEnPassant()){
                return true; // en passant
        }
    }

    if (abs(direction.row) == 2 && direction.col == 0){
        if (dynamic_cast<Pawn*>(getPiece(i))->getIsFirstMove() && !getPiece(f)){
            return true;
        }
    }

    if (abs(direction.row) == 1 && direction.col == 0) {
        if (!getPiece(f)){
            return true;
        }
    }

    return false;
}

bool Board::isValidMove(const Position& i, const Position& f, const bool& turn) const {
    std::vector<Position> possibleMoves = getPiece(i)->getPossibleMoves(i);
    Position kingPosition = (turn) ? getWhiteKing() : getBlackKing();

    if (!moveFound(possibleMoves, f)) {return false;} // checks if move matches behavior of piece

    if (getPiece(f) && getPiece(f)->getColor() == getPiece(i)->getColor()) {return false;} // checks for empty space or opposite color

    if (!isMovePathClear(i, f) && getPiece(i)->getType() != PieceType::Knight) {return false;} // checks if path is clear or knight

    // if king is moving update its position
    if (getPiece(i)->getType() == PieceType::King){
        kingPosition = f;
    }

    // check if new position leaves king in check
    Board simulatedBoard(*this);
    delete simulatedBoard.getPiece(f);
    simulatedBoard.getPiece(f) = simulatedBoard.getPiece(i);
    simulatedBoard.getPiece(i) = nullptr;
    if (simulatedBoard.isKingInCheck(kingPosition)){return false;}

    // final checks for castle and en passant
    switch (getPiece(i)->getType()){
        case PieceType::Pawn: 
            return pawnMoveValidation(i, f);

        case PieceType::King:
            return kingMoveValidation(i, f, simulatedBoard);

        default:
            return true;
    }
}

bool Board::isKingInCheck(const Position& king) const {
    bool kingColor = getPiece(king)->getColor();
    bool oppColor = !kingColor;

    // pawn checks
    int pawnAttackDirection = (kingColor) ? -1 : 1;
    Position pawnAttackDirections[] = {{pawnAttackDirection * 1, 1}, {pawnAttackDirection * 1, -1}};
    for (const auto& dir : pawnAttackDirections){
        Position pawn = king + dir;
        if (isWithinBounds(pawn) && getPiece(pawn) && getPiece(pawn)->getType() == PieceType::Pawn && getPiece(pawn)->getColor() == oppColor){
            return true;
        }
    }

    // knight checks
    Position knightAttackDirections[] = {{1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};
    for (const auto& dir : knightAttackDirections){
        Position knight = king + dir;
        if (isWithinBounds(knight) && getPiece(knight) && getPiece(knight)->getType() == PieceType::Knight && getPiece(knight)->getColor() == oppColor){
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

void Board::movePiece(const Position& i, const Position& f, const bool& turn) {

    if (isValidMove(i, f, turn)){
        if (getPiece(i)->getType() == PieceType::King){ // castling
            (turn) ? updateWhiteKing(f) : updateBlackKing(f); // update king position

            if (abs(f.col - i.col) == 2){
                int castleDirection = (f.col > i.col) ? 1 : -1;
                Position iRookPosition = (castleDirection == 1) ? Position{i.row, MAX_HEIGHT - 1} : Position{i.row, 0};
                Position fRookPosition = {i.row, i.col + castleDirection};

                delete getPiece(fRookPosition);
                getPiece(fRookPosition) = getPiece(iRookPosition);
                getPiece(iRookPosition) = nullptr;
            }

        }
        else if (getPiece(i)->getType() == PieceType::Pawn){ // en passant
            if (getPiece({i.row, f.col}) && getPiece({i.row, f.col})->getType() == PieceType::Pawn &&
            getPiece({i.row, f.col})->getColor() != getPiece(i)->getColor() &&
            dynamic_cast<Pawn*>(getPiece({i.row, f.col}))->getEnPassant()){
                delete getPiece({i.row, f.col});
                getPiece({i.row, f.col}) = nullptr;
            }
        }

        delete getPiece(f);
        getPiece(f) = getPiece(i);
        getPiece(i) = nullptr;

    }
}

void Board::promotePawn(const Position& p, const char& type){
    bool color = getPiece(p)->getColor();
    delete getPiece(p);
    
    switch (type){
        case 'B':
            getPiece(p) = new Bishop(color);
            break;

        case 'Q':
            getPiece(p) = new Queen(color);
            break;

        case 'N':
            getPiece(p) = new Knight(color);
            break;

        case 'R':
            getPiece(p) = new Rook(color);
            break;
    }
}

bool Board::isPawnPromotion(const Position& p) const {
    return (*this).getPiece(p)->getType() == PieceType::Pawn && (p.row == MAX_HEIGHT || p.row == MIN_HEIGHT);
}

std::vector<std::pair<Position, Position>> Board::getLegalMoves(const bool& turn) const {
    std::vector<std::pair<Position, Position>> legalMoves;
    legalMoves.reserve(75);
    Color color = (turn) ? Color::White : Color::Black;

    for (int i = 0; i < MAX_HEIGHT; i++){
        for (int j = 0; j < MAX_WIDTH; j++){
            if ((*this).getPiece({i, j}) && (*this).getPiece({i, j})->getColor() == color){
                std::vector<Position> possibleMoves = (*this).getPiece({i, j})->getPossibleMoves({i, j});
                for (const auto& move : possibleMoves){
                    if (isValidMove({i, j}, move, turn)){
                        legalMoves.push_back({{i, j}, move});
                    }
                }
            }
        }
    }

    return legalMoves;
}

bool Board::isCheckmate(const bool& turn) const {
    Position king = (turn) ? getWhiteKing() : getBlackKing();

    // check if king is in check
    if (!isKingInCheck(king)){
        return false;
    }

    if (!getLegalMoves(turn).empty()){
        return false;
    }

    return true;
}

bool Board::isDraw(const bool& turn) const {
    Position king = (turn) ? getWhiteKing() : getBlackKing();

    if (!isKingInCheck(king) && getLegalMoves(turn).empty()){
        return true;
    }

    // insufficient material check
    std::pair<int, int> material = countMaterial();
    if ((material.first == 0) || (material.first == 1 && material.second == 1)){
        return true;
    }

    return false;
}

std::pair<int, int> Board::countMaterial() const {
    std::pair<int, int> material = {0, 0}; // {total, minor}

    for (int i = 0; i < MAX_HEIGHT; i++){
        for (int j = 0; j < MAX_WIDTH; j++){
            if ((*this).getPiece({i, j})){
                if ((*this).getPiece({i, j})->getType() != PieceType::King){
                    material.first++;
                }
                if (((*this).getPiece({i, j})->getType() == PieceType::Bishop) || ((*this).getPiece({i, j})->getType() == PieceType::Knight)){
                    material.second++;
                }
                // Break early if there is sufficient material
                if (material.first > 1 || (material.first == 1 && material.second > 1)){
                    break;
                }
            }
        }
    }

    return material;
}