#include "piece.h"

std::vector<Position> Piece::generateMoves(const std::vector<Position>& directions, const Position& i) const{
    std::vector<Position> moves;

    for (const auto& dir : directions){
        Position f = i + dir;
        if (isWithinBounds(f)){
            moves.push_back(f);
        }
    }

    return moves;
}

std::vector<Position> King::getPossibleMoves(const Position& i) const {
    std::vector<Position> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    std::vector<Position> moves = std::move(generateMoves(directions, i));

    if (this->getCastle()){ // castle
        Position castles[] = {{0, 2}, {0, -2}};
    
        for (const auto& castle : castles){
            Position f = i + castle;
            moves.push_back(f);
        }
    }

    return moves;
}   

std::vector<Position> Knight::getPossibleMoves(const Position& i) const {
    std::vector<Position> directions = {{1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};

    std::vector<Position> moves = std::move(generateMoves(directions, i));

    return moves;
}

std::vector<Position> Pawn::getPossibleMoves(const Position& i) const {
    int direction = (this->getColor()) ? -1 : 1;
    std::vector<Position> directions = {{direction * 1, 0}, {direction * 1, 1}, {direction * 1, -1}};

    std::vector<Position> moves = std::move(generateMoves(directions, i));

    if (this->getIsFirstMove()){
        Position pushTwo = {direction * 2, 0};
        moves.push_back(i + pushTwo);
    }

    return moves;
}

std::vector<Position> Rook::getPossibleMoves(const Position& i) const {
    std::vector<Position> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    std::vector<Position> moves = std::move(generateMoves(directions, i));

    return moves;
}

std::vector<Position> Bishop::getPossibleMoves(const Position& i) const {
    std::vector<Position> directions = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};

    std::vector<Position> moves = std::move(generateMoves(directions, i));

    return moves;
}

std::vector<Position> Queen::getPossibleMoves(const Position& i) const {
    std::vector<Position> directions = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}, {0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    std::vector<Position> moves = std::move(generateMoves(directions, i));

    return moves;
}

char to_char(const PieceType& piece){ // function to convert PieceType enum to char for printBoard()
    switch (piece){
        case PieceType::Bishop:
        return 'B';
        break;

        case PieceType::Rook:
        return 'R';
        break;

        case PieceType::Queen:
        return 'Q';
        break;

        case PieceType::King:
        return 'K';
        break;

        case PieceType::Knight:
        return 'N';
        break;

        case PieceType::Pawn:
        return 'P';
        break;
    }

    return 0;
}

bool isWithinBounds(const Position& p){
    return (p.row < MAX_HEIGHT && p.col < MAX_WIDTH && p.row >= MIN_HEIGHT && p.col >= MIN_WIDTH);
}