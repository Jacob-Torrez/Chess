#include "board.h"

constexpr int PLIES = 4;

struct Node {
    int value;
    std::pair<Position, Position> move;

    std::vector<Node*> children;

    Node (std::pair<Position, Position> m) : move(m) {}
};

class Engine {
public:
    Engine() : root(nullptr) {}
    ~Engine();

    std::pair<Position, Position> getBestMove(Board& board, int depth, bool turn);
    

private:
    void deleteTree (Node* node);
    int minimax(Board& board, Node* node, int depth, int alpha, int beta, bool turn);
    int evaluateGameState(const Board& board, bool turn) const;

    static const int positionalTable[MAX_HEIGHT][MAX_WIDTH];
    Node* root;

};