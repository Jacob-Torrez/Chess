#include "board.h"

Board::Board() : whiteKing({7,4}), blackKing({0,4}) {
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

    whiteKing = other.whiteKing;
    blackKing = other.blackKing;
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

    whiteKing = other.whiteKing;
    blackKing = other.blackKing;

    return *this;
}

bool Board::possibleMoveFound(const std::vector<Position>& moves, const Position& move) const {
    for (const auto& m : moves){
        if (m == move){
            return true;
        }
    }

    return false;
}

bool Board::checkAttackDirection(const Position& king, const Position& dir, const bool& kingColor, const bool& oppColor, const PieceType& type1, const PieceType& type2) const {
    Position current = king + dir;
        while (isWithinBounds(current)){
            if (readPiece(current)){
                if (readPiece(current)->getColor() == oppColor && (readPiece(current)->getType() == type1 || readPiece(current)->getType() == type2)){
                    return true;
                }
                else {
                    break;
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
            if (readPiece({i, j})){
                if (readPiece({i, j})->getColor() == Color::White){
                    std::cout << " | " << to_char(readPiece({i, j})->getType());
                }
                else{
                    std::cout << " | " << static_cast<char>(tolower(to_char(readPiece({i, j})->getType())));
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

bool Board::checkMovePath(const Position& i, const Position& f) const {
    int rowStep = (f.row > i.row) - (f.row < i.row); // gets direction as 0, 1, or -1
    int colStep = (f.col > i.col) - (f.col < i.col);

    Position step(rowStep, colStep);

    if (i.col != f.col && i.row != f.row && abs(f.row - i.row) != abs(f.col - i.col)){ // makes sure path is hori, vert, or diag
        return false;
    }

    Position current = i + step;
    while (current != f){
        if (readPiece(current)){
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
        if (!readPiece(rookPosition) || readPiece(rookPosition)->getType() != PieceType::Rook || !dynamic_cast<Rook*>(readPiece(rookPosition))->getCastle()) {
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
        if (castleDirection == -1 && !checkMovePath(rookPosition, {i.row, i.col - 1})) {
            return false;
        }
    }
    
    return true;
}

bool Board::pawnMoveValidation(const Position& i, const Position& f) const {
    Position direction = {f.row - i.row, f.col - i.col};

    // check for diagonal move
    if (abs(direction.col) == 1 && abs(direction.row) == 1){
        if (readPiece(f) && readPiece(f)->getColor() != readPiece(i)->getColor()){
            return true; // standard capture
        }

        if (readPiece({i.row, f.col}) && readPiece({i.row, f.col})->getType() == PieceType::Pawn &&
            readPiece({i.row, f.col})->getColor() != readPiece(i)->getColor() &&
            dynamic_cast<Pawn*>(readPiece({i.row, f.col}))->getEnPassant()){
                return true; // en passant
        }
    }

    if (abs(direction.row) == 2 && direction.col == 0){
        if (dynamic_cast<Pawn*>(readPiece(i))->getIsFirstMove() && !readPiece(f)){
            return true;
        }
    }

    if (abs(direction.row) == 1 && direction.col == 0) {
        if (!readPiece(f)){
            return true;
        }
    }

    return false;
}

bool Board::isValidMove(const Position& i, const Position& f, const bool& turn) const {
    std::vector<Position> possibleMoves = readPiece(i)->getPossibleMoves(i);

    if (!possibleMoveFound(possibleMoves, f)) {return false;} // checks if move matches behavior of piece

    if (readPiece(f) && readPiece(f)->getColor() == readPiece(i)->getColor()) {return false;} // checks for empty space or opposite color

    if (!checkMovePath(i, f) && readPiece(i)->getType() != PieceType::Knight) {return false;} // checks if path is clear or knight

    // check if new position leaves king in check
    Board simulatedBoard(*this);
    delete simulatedBoard.getPiece(f);
    simulatedBoard.getPiece(f) = simulatedBoard.getPiece(i);
    simulatedBoard.getPiece(i) = nullptr;

    if (readPiece(i)->getType() == PieceType::King) {
        (turn ? simulatedBoard.whiteKing : simulatedBoard.blackKing) = f;
    }

    Position kingPosition = (turn) ? simulatedBoard.whiteKing : simulatedBoard.blackKing;

    if (simulatedBoard.isKingInCheck(kingPosition)){return false;}

    // final checks for castle and en passant
    switch (readPiece(i)->getType()){
        case PieceType::Pawn:
            return pawnMoveValidation(i, f);

        case PieceType::King:
            return kingMoveValidation(i, f, simulatedBoard);

        default:
            return true;
    }
}

bool Board::isKingInCheck(const Position& king) const {
    bool kingColor = readPiece(king)->getColor();
    bool oppColor = !kingColor;

    // pawn checks
    int pawnAttackDirection = (kingColor) ? -1 : 1;
    Position pawnAttackDirections[] = {{pawnAttackDirection * 1, 1}, {pawnAttackDirection * 1, -1}};
    for (const auto& dir : pawnAttackDirections){
        Position pawn = king + dir;
        if (isWithinBounds(pawn) && readPiece(pawn) && readPiece(pawn)->getType() == PieceType::Pawn && readPiece(pawn)->getColor() == oppColor){
            return true;
        }
    }

    // knight checks
    Position knightAttackDirections[] = {{1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};
    for (const auto& dir : knightAttackDirections){
        Position knight = king + dir;
        if (isWithinBounds(knight) && readPiece(knight) && readPiece(knight)->getType() == PieceType::Knight && readPiece(knight)->getColor() == oppColor){
            return true;
        }
    }

    // bishop/queen checks
    std::vector<Position> diagonalAttackDirections = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
    for (const auto& dir : diagonalAttackDirections){
        if (checkAttackDirection(king, dir, kingColor, oppColor, PieceType::Bishop, PieceType::Queen)){
            return true;
        }
    }

    // queen/rook checks
    std::vector<Position> orthogonalAttackDirections = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    for (const auto& dir : orthogonalAttackDirections){
        if (checkAttackDirection(king, dir, kingColor, oppColor, PieceType::Rook, PieceType::Queen)){
            return true;
        }
    }

    return false;
}

MoveInfo Board::movePiece(const Position& i, const Position& f, const bool& turn) {
    MoveInfo moveInfo = {};

    // saving moved piece's position
    moveInfo.start = i;
    moveInfo.end = f;

    // copying moved piece
    moveInfo.movedPiece = getPiece(i)->clone();

    // cloning captured piece and saving position
    if (getPiece(f)){
        moveInfo.capturedPiece = getPiece(f)->clone();
        moveInfo.capturedPositionStart = f;
    }

    if (getPiece(i)->getType() == PieceType::King){ // castling
        ((turn) ? whiteKing : blackKing) = f; // update king position
        if (abs(f.col - i.col) == 2){
            int castleDirection = (f.col > i.col) ? 1 : -1;
            Position rookInitial = (castleDirection == 1) ? Position{i.row, MAX_HEIGHT - 1} : Position{i.row, 0};
            Position rookFinal = {i.row, i.col + castleDirection};

            // saving castled rook's initial and final position and cloning rook
            moveInfo.castlingMove = true;
            moveInfo.capturedPositionStart = rookInitial;
            moveInfo.capturedPiece = getPiece(moveInfo.capturedPositionStart)->clone();
            moveInfo.capturedPositionEnd = rookFinal;

            delete getPiece(rookFinal);
            getPiece(rookFinal) = getPiece(rookInitial);
            getPiece(rookInitial) = nullptr;
        }
    }
    else if (getPiece(i)->getType() == PieceType::Pawn){ // en passant
        Position enPassantPos = {i.row, f.col};
        if (getPiece(enPassantPos) && getPiece(enPassantPos)->getType() == PieceType::Pawn &&
        getPiece(enPassantPos)->getColor() != getPiece(i)->getColor() &&
        dynamic_cast<Pawn*>(getPiece(enPassantPos))->getEnPassant()){
            
            // cloning captured pawn and saving position
            moveInfo.enPassantMove = true;
            moveInfo.capturedPiece = getPiece(enPassantPos)->clone();
            moveInfo.capturedPositionStart = enPassantPos;

            delete getPiece(enPassantPos);
            getPiece(enPassantPos) = nullptr;
        }
    }

    disableEnPassant(moveInfo);

    delete getPiece(f);
    getPiece(f) = getPiece(i);
    getPiece(i) = nullptr;

    // update special rules (eg. en passsant, castling, first move)
    updateSpecialRules(i, f, moveInfo);

    // check for pawn promotion
    if (getPiece(f)->getType() == PieceType::Pawn && (f.row == (MAX_HEIGHT - 1) || f.row == MIN_HEIGHT)){
        bool color = getPiece(f)->getColor();
        delete getPiece(f);
        getPiece(f) = new Queen(color);
    }

    return moveInfo;
}

std::vector<std::pair<Position, Position>> Board::getLegalMoves(const bool& turn) const {
    std::vector<std::pair<Position, Position>> legalMoves;
    legalMoves.reserve(75);
    Color color = (turn) ? Color::White : Color::Black;

    for (int i = 0; i < MAX_HEIGHT; i++){
        for (int j = 0; j < MAX_WIDTH; j++){
            if (readPiece({i, j}) && readPiece({i, j})->getColor() == color){
                std::vector<Position> possibleMoves = readPiece({i, j})->getPossibleMoves({i, j});
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
    Position king = (turn) ? whiteKing : blackKing;

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
    Position king = (turn) ? whiteKing : blackKing;

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
            if (readPiece({i, j})){
                if (readPiece({i, j})->getType() != PieceType::King){
                    material.first++;
                }
                if ((readPiece({i, j})->getType() == PieceType::Bishop) || (readPiece({i, j})->getType() == PieceType::Knight)){
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

void Board::updateSpecialRules(const Position& i, const Position& f, MoveInfo& moveInfo){

    if (getPiece(f)->getType() == PieceType::Rook){
        dynamic_cast<Rook*>(getPiece(f))->updateCastle();
    }

    if (getPiece(f)->getType() == PieceType::King){
        dynamic_cast<King*>(getPiece(f))->updateCastle();
    }

    if (getPiece(f)->getType() == PieceType::Pawn){
        dynamic_cast<Pawn*>(getPiece(f))->updateIsFirstMove();
        if (abs(f.row - i.row) == 2){
            dynamic_cast<Pawn*>(getPiece(f))->enableEnPassant();
        }
    }
}

void Board::undoMove(MoveInfo& moveInfo){

    delete getPiece(moveInfo.start);
    getPiece(moveInfo.start) = moveInfo.movedPiece;

    delete getPiece(moveInfo.end);
    getPiece(moveInfo.end) = nullptr;

    if (moveInfo.castlingMove){  // castling
        delete getPiece(moveInfo.capturedPositionEnd);
        getPiece(moveInfo.capturedPositionStart) = moveInfo.capturedPiece;
        getPiece(moveInfo.capturedPositionEnd) = nullptr;
    }
    else if (moveInfo.capturedPiece || moveInfo.enPassantMove){
        delete getPiece(moveInfo.capturedPositionStart);
        getPiece(moveInfo.capturedPositionStart) = moveInfo.capturedPiece;
    }

    if (moveInfo.movedPiece->getType() == PieceType::King){
        (moveInfo.movedPiece->getColor()) ? whiteKing = moveInfo.start : blackKing = moveInfo.start;
    }

    revertEnPassant(moveInfo);

    moveInfo.capturedPiece = nullptr;
    moveInfo.movedPiece = nullptr;
    moveInfo.enPassantUpdates.clear();
}

void Board::revertEnPassant(const MoveInfo& moveInfo){
    for (const auto& pos : moveInfo.enPassantUpdates){
        if (getPiece(pos)){
            dynamic_cast<Pawn*>(getPiece(pos))->enableEnPassant();
        }
    }
}

void Board::disableEnPassant(MoveInfo& moveInfo){
    for (int j = 0; j < MAX_WIDTH; j++){

        // disables en passants from last turn and saves updated pawns into 
        if (getPiece({4, j}) && getPiece({4, j})->getType() == PieceType::Pawn){
            dynamic_cast<Pawn*>(getPiece({4, j}))->disableEnPassant();
            moveInfo.enPassantUpdates.push_back({4,j});
        }
        if (getPiece({3, j}) && getPiece({3, j})->getType() == PieceType::Pawn){
            dynamic_cast<Pawn*>(getPiece({3, j}))->disableEnPassant();
            moveInfo.enPassantUpdates.push_back({3,j});
        }
    }
}