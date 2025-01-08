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
            const Piece* piece = getPiece(current);
            if (piece){
                if (piece->getColor() == kingColor){
                    break;
                }
                else if (piece->getColor() == oppColor && (piece->getType() == type1 || piece->getType() == type2)){
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
            Piece* piece = getPiece({i, j});
            if (piece){
                if (piece->getColor() == Color::White){
                    std::cout << " | " << to_char(piece->getType());
                }
                else{
                    std::cout << " | " << static_cast<char>(tolower(to_char(piece->getType())));
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
        Piece* rook = getPiece(rookPosition);
        if (!rook || rook->getType() != PieceType::Rook || !dynamic_cast<Rook*>(rook)->getCastle()) {
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
    Piece* initial = getPiece(i);
    Piece* final = getPiece(f);

    // check for diagonal move
    if (abs(direction.col) == 1 && abs(direction.row) == 1){
        if (final && final->getColor() != initial->getColor()){
            return true; // standard capture
        }

        Piece* enPassant = getPiece({i.row, f.col});
        if (enPassant && enPassant->getType() == PieceType::Pawn &&
            enPassant->getColor() != initial->getColor() &&
            dynamic_cast<Pawn*>(enPassant)->getEnPassant()){
                return true; // en passant
        }
    }

    if (abs(direction.row) == 2 && direction.col == 0){
        if (dynamic_cast<Pawn*>(initial)->getIsFirstMove() && !final){
            return true;
        }
    }

    if (abs(direction.row) == 1 && direction.col == 0) {
        if (!final){
            return true;
        }
    }

    return false;
}

bool Board::isValidMove(const Position& i, const Position& f, const bool& turn) const {
    Piece* initial = getPiece(i);
    Piece* final = getPiece(f);
    std::vector<Position> possibleMoves = initial->getPossibleMoves(i);
    Position kingPosition = (turn) ? whiteKing : blackKing;

    if (!possibleMoveFound(possibleMoves, f)) {return false;} // checks if move matches behavior of piece

    if (final  && final ->getColor() == initial->getColor()) {return false;} // checks for empty space or opposite color

    if (!checkMovePath(i, f) && initial->getType() != PieceType::Knight) {return false;} // checks if path is clear or knight

    // if king is moving update its position
    if (initial->getType() == PieceType::King){
        kingPosition = f;
    }

    // check if new position leaves king in check
    Board simulatedBoard(*this);
    delete simulatedBoard.getPiece(f);
    simulatedBoard.getPiece(f) = simulatedBoard.getPiece(i);
    simulatedBoard.getPiece(i) = nullptr;
    if (simulatedBoard.isKingInCheck(kingPosition)){return false;}

    // final checks for castle and en passant
    switch (initial->getType()){
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

    disableEnPassant(moveInfo);

    // saving moved piece's position
    moveInfo.start = i;
    moveInfo.end = f;

    Piece* initial = getPiece(i);
    Piece* final = getPiece(f);

    // copying moved piece
    moveInfo.movedPiece = initial->clone();

    // cloning captured piece and saving position
    if (final){
        moveInfo.capturedPiece = final->clone();
        moveInfo.capturedPositionStart = f;
    }

    if (initial->getType() == PieceType::King){ // castling
        ((turn) ? whiteKing : blackKing) = f; // update king position
        if (abs(f.col - i.col) == 2){
            int castleDirection = (f.col > i.col) ? 1 : -1;
            Piece* rookInitial = getPiece((castleDirection == 1) ? Position{i.row, MAX_HEIGHT - 1} : Position{i.row, 0});
            Piece* rookFinal = getPiece({i.row, i.col + castleDirection});

            // saving castled rook's initial and final position and cloning rook
            moveInfo.castlingMove = true;
            moveInfo.capturedPositionStart = (castleDirection == 1) ? Position{i.row, MAX_HEIGHT - 1} : Position{i.row, 0};
            moveInfo.capturedPiece = getPiece(moveInfo.capturedPositionStart)->clone();
            moveInfo.capturedPositionEnd = {i.row, i.col + castleDirection};

            delete rookFinal;
            rookFinal = initial;
            initial = nullptr;
        }
    }
    else if (initial->getType() == PieceType::Pawn){ // en passant
        Piece* enPassant = getPiece({i.row, f.col});
        if (enPassant && enPassant->getType() == PieceType::Pawn &&
        enPassant->getColor() != initial->getColor() &&
        dynamic_cast<Pawn*>(enPassant)->getEnPassant()){
            
            // cloning captured pawn and saving position
            moveInfo.enPassantMove = true;
            moveInfo.capturedPiece = enPassant->clone();
            moveInfo.capturedPositionStart = {i.row, f.col};

            delete enPassant;
            enPassant = nullptr;
        }
    }
    delete final;
    final = initial;
    initial = nullptr;

    // update special rules (eg. en passsant, castling, first move)
    updateSpecialRules(i, f, moveInfo);

    // check for pawn promotion
    if (final->getType() == PieceType::Pawn && (f.row == MAX_HEIGHT || f.row == MIN_HEIGHT)){
        bool color = final->getColor();
        delete final;
        final = new Queen(color);
    }

    return moveInfo;
}

std::vector<std::pair<Position, Position>> Board::getLegalMoves(const bool& turn) const {
    std::vector<std::pair<Position, Position>> legalMoves;
    legalMoves.reserve(75);
    Color color = (turn) ? Color::White : Color::Black;

    for (int i = 0; i < MAX_HEIGHT; i++){
        for (int j = 0; j < MAX_WIDTH; j++){
            Piece* piece = getPiece({i, j});
            if (piece && piece->getColor() == color){
                std::vector<Position> possibleMoves = piece->getPossibleMoves({i, j});
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
            Piece* piece = getPiece({i, j});
            if (piece){
                if (piece->getType() != PieceType::King){
                    material.first++;
                }
                if ((piece->getType() == PieceType::Bishop) || (piece->getType() == PieceType::Knight)){
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

    Piece* final = getPiece(f);
    if (final->getType() == PieceType::Rook){
        dynamic_cast<Rook*>(final)->updateCastle();
    }

    if (final->getType() == PieceType::King){
        dynamic_cast<King*>(final)->updateCastle();
    }

    if (final->getType() == PieceType::Pawn){
        dynamic_cast<Pawn*>(final)->updateIsFirstMove();
        if (abs(f.row - i.col) == 2){
            dynamic_cast<Pawn*>(final)->enableEnPassant();
        }
    }
}

void Board::undoMove(MoveInfo& moveInfo){
    Piece* start = getPiece(moveInfo.start);
    Piece* end = getPiece(moveInfo.end);
    Piece* capturedPieceStart = getPiece(moveInfo.capturedPositionStart);
    Piece* capturedPieceEnd = getPiece(moveInfo.capturedPositionEnd);

    delete end;
    start = moveInfo.movedPiece;
    end = nullptr;

    if (moveInfo.movedPiece->getType() == PieceType::King){ // castling
        if (moveInfo.castlingMove){
            delete capturedPieceEnd;
            capturedPieceStart = moveInfo.capturedPiece;
            capturedPieceEnd = nullptr;
        }
    }
    else if (moveInfo.movedPiece->getType() == PieceType::Pawn){ // en passant
        if (moveInfo.enPassantMove){
            delete capturedPieceStart;
            capturedPieceStart = moveInfo.capturedPiece;
        }
    }
    else if (moveInfo.capturedPiece){
        delete capturedPieceStart;
        capturedPieceStart = moveInfo.capturedPiece;
    }

    revertEnPassant(moveInfo);

    moveInfo.movedPiece = nullptr;
    moveInfo.capturedPiece = nullptr;
}

void Board::revertEnPassant(const MoveInfo& moveInfo){
    for (const auto& pos : moveInfo.enPassantUpdates){
        Piece* pawn = getPiece(pos);
        dynamic_cast<Pawn*>(pawn)->enableEnPassant();
    }
}

void Board::disableEnPassant(MoveInfo& moveInfo){
    for (int j = 0; j < MAX_WIDTH; j++){
        Piece* fourthRank = getPiece({4, j});
        Piece* fifthRank = getPiece({5, j});

        // disables en passants from last turn and saves updated pawns into 
        if (fourthRank && fourthRank->getType() == PieceType::Pawn){
            dynamic_cast<Pawn*>(fourthRank)->disableEnPassant();
            moveInfo.enPassantUpdates.push_back({4,j});
        }
        if (fifthRank && fifthRank->getType() == PieceType::Pawn){
            dynamic_cast<Pawn*>(fifthRank)->disableEnPassant();
            moveInfo.enPassantUpdates.push_back({5,j});
        }
    }
}