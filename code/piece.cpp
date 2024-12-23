#include "piece.h"
#include "board.h"

std::vector<Position> King::getPossibleMoves(const Position& i, const Board& board){
    std::vector<Position> moves;
    Position directions[] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    for (const auto& dir : directions){
        Position f = i + dir;
        if (f.row < MAX_HEIGHT && f.col < MAX_WIDTH && f.row >= MIN_HEIGHT && f.col >= MIN_WIDTH){
            if (!board[f] || board[i]->getColor() != board[f]->getColor()){
                moves.push_back(f);
            }
        }
    }

    if (this->getCastle()){ // castle
        Position castles[] = {{0, 2}, {0, -2}};
    
        for (const auto& castle : castles){
            Position f = i + castle;
            if (board.isPathClear(i, f)){
                moves.push_back(f);
            }
        }
    }

    return moves;
}   

std::vector<Position> Knight::getPossibleMoves(const Position& i, const Board& board){
    std::vector<Position> moves;
    Position directions[] = {{1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};

    for (const auto& dir : directions){
        Position f = i + dir;
        if (f.row < MAX_HEIGHT && f.col < MAX_WIDTH && f.row >= MIN_HEIGHT && f.col >= MIN_WIDTH){
            if (!board[f] || board[i]->getColor() != board[f]->getColor()){
                moves.push_back(f);
            }
        }
    }

    return moves;
}

std::vector<Position> Pawn::getPossibleMoves(const Position& i, const Board& board){
    std::vector<Position> moves;
    int direction = (this->getColor()) ? -1 : 1;
    Position directions[] = {{direction * 1, 0}, {direction * 1, 1}, {direction * 1, -1}};

    for (int j = 0; j < 3; j++){
        Position f = i + directions[j];
        if (f.row < MAX_HEIGHT && f.col < MAX_WIDTH && f.row >= MIN_HEIGHT && f.col >= MIN_WIDTH){
            switch (j){
                case 0: // forward move
                    if (!board[f]){
                        moves.push_back(f);
                    }
                    break;

                case 1:
                    if (board[f] && (board[i]->getColor() != board[f]->getColor())){ // right diagonal capture
                        moves.push_back(f);
                    } 
                    // right en passant
                    else if (board[i + Position(0, 1)] && (board[i + Position(0, 1)]->getType() == PieceType::Pawn) && (board[i + Position(0, 1)]->getColor() != board[i]->getColor())){
                        moves.push_back(f);
                    }
                    break;

                case 2:
                    if (board[f] && (board[i]->getColor() != board[f]->getColor())){ // left diagonal capture
                        moves.push_back(f);
                    }
                    // left en passant
                    else if (board[i + Position(0, -1)] && (board[i + Position(0, -1)]->getType() == PieceType::Pawn) && (board[i + Position(0, -1)]->getColor() != board[i]->getColor())){ 
                        moves.push_back(f);
                    }
                    break;
            }
        }
    }

    return moves;
}

std::vector<Position> Rook::getPossibleMoves(const Position& i, const Board& board){
    std::vector<Position> moves;
    Position directions[] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    for (const auto& dir : directions){
        Position f = i + dir;
        bool stop;
        do {
            stop = true;
            if (f.row < MAX_HEIGHT && f.col < MAX_WIDTH && f.row >= MIN_HEIGHT && f.col >= MIN_WIDTH){
                if (!board[f]){
                    moves.push_back(f);
                    stop = false;
                }
                else if (board[i]->getColor() != board[f]->getColor()){
                    moves.push_back(f);
                }
            }

            f += dir;
        } while (!stop);
        
    }

    return moves;
}

std::vector<Position> Bishop::getPossibleMoves(const Position& i, const Board& board){
    std::vector<Position> moves;
    Position directions[] = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};

    for (const auto& dir : directions){
        Position f = i + dir;
        bool stop;
        do {
            stop = true;
            if (f.row < MAX_HEIGHT && f.col < MAX_WIDTH && f.row >= MIN_HEIGHT && f.col >= MIN_WIDTH){
                if (!board[f]){
                    moves.push_back(f);
                    stop = false;
                }
                else if (board[i]->getColor() != board[f]->getColor()){
                    moves.push_back(f);
                }
            }

            f += dir;
        } while (!stop);
        
    }

    return moves;
}

std::vector<Position> Queen::getPossibleMoves(const Position& i, const Board& board){
    std::vector<Position> moves;
    Position directions[] = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}, {0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    for (const auto& dir : directions){
        Position f = i + dir;
        bool stop;
        do {
            stop = true;
            if (f.row < MAX_HEIGHT && f.col < MAX_WIDTH && f.row >= MIN_HEIGHT && f.col >= MIN_WIDTH){
                if (!board[f]){
                    moves.push_back(f);
                    stop = false;
                }
                else if (board[i]->getColor() != board[f]->getColor()){
                    moves.push_back(f);
                }
            }

            f += dir;
        } while (!stop);
        
    }

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

bool moveFound(const vector<Position>& moves, const Position& move){
    for (const auto& m : moves){
        if (m == move){
            return true;
        }
    }

    return false;
}