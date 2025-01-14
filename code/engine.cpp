#include "engine.h"
#include <climits>

Engine::~Engine(){
    if (root) {
        deleteTree(root);
        root = nullptr;
    }  
}

void Engine::deleteTree(Node* node) {
    for (const auto& child : node->children){
        deleteTree(child);
    }

    delete node;
}

int Engine::minimax(Board& board, Node* node, int depth, int alpha, int beta, bool turn){
    if (depth == 0 || board.isCheckmate(turn) || board.isDraw(turn)){
        node->value = evaluateGameState(board, turn);
        return node->value;
    }
    std::vector<std::pair<Position, Position>> legalMoves = board.getLegalMoves(turn);

    if (turn){
        int maxEval = INT_MIN;
        for (int i = 0; i < legalMoves.size(); i++){
            const std::pair<Position, Position>& move = legalMoves[i];
            
            // generate child node
            MoveInfo moveInfo = board.movePiece(move.first, move.second, turn);
            Node* tempNode = new Node(move);
    
            // evaluate with minimax and alpha-beta
            int eval = minimax(board, tempNode, depth - 1, alpha, beta, !turn);
            board.undoMove(moveInfo);

            node->children.push_back(tempNode);

            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {break;}
        }
        node->value = maxEval;
        return maxEval;
    }
    else {
        int minEval = INT_MAX;
        for (int i = 0; i < legalMoves.size(); i++){
            const std::pair<Position, Position>& move = legalMoves[i];
            // generate child node
            MoveInfo moveInfo = board.movePiece(move.first, move.second, turn);
            Node* tempNode = new Node(move);
    
            // evaluate with minimax and alpha-beta
            int eval = minimax(board, tempNode, depth - 1, alpha, beta, !turn);
            board.undoMove(moveInfo);

            node->children.push_back(tempNode);

            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) {break;}
        }
        node->value = minEval;
        return minEval;
    }
}

int Engine::evaluateGameState(const Board& board, bool turn) const {
    if (board.isCheckmate(turn)){
        return (turn) ? INT_MIN : INT_MAX;
    }
    if (board.isDraw(turn)){
        return 0;
    }

    int score = 0;
    for (int i = 0; i < MAX_HEIGHT; i++) {
        for (int j = 0; j < MAX_WIDTH; j++) {
            Piece* piece = board.readPiece({i, j});
            if (piece) {
                int value = 0;
                int positionalValue = positionalTable[i][j];

                switch (piece->getType()) {
                    case PieceType::Pawn:   value = 1; break;
                    case PieceType::Bishop: value = 3; break;
                    case PieceType::Knight: value = 3; break;
                    case PieceType::Rook:   value = 5; break;
                    case PieceType::Queen:  value = 9; break;
                    default: break;
                }
                score += (piece->getColor() ? value + positionalValue : -(value + positionalValue));
            }
        }
    }

    return score;
}

std::pair<Position, Position> Engine::getBestMove(Board& board, int depth, bool turn){
    if (root != nullptr){
        deleteTree(root);
    }

    root = new Node({{-1, -1}, {-1, -1}});

    minimax(board, root, PLIES, INT_MIN, INT_MAX, turn);

    std::pair<Position, Position> bestMove;
    for (const auto& child : root->children){
        if (root->value == child->value){
            bestMove = child->move;
        }
    }

    deleteTree(root);
    root = nullptr;

    return bestMove;
}

const int Engine::positionalTable[MAX_HEIGHT][MAX_WIDTH] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 2, 2, 2, 2, 1, 0},
    {0, 1, 2, 3, 3, 2, 1, 0},
    {0, 1, 2, 3, 3, 2, 1, 0},
    {0, 1, 2, 2, 2, 2, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
};