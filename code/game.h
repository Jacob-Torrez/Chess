#pragma once
#include "board.h"
#include "engine.h"
#include <string>

class Game {
public:
    Game() : turn(Color::White), gameOver(false), board() {};
    void run();

private:
    Board board;
    bool gameOver;
    bool turn; // 1 is white, 0 is black

    void playEngine();
    void playPlayer();
    Position parsePosition(const std::string& s) const;
    std::pair<Position, Position> getPlayerMove();
    bool isValidInput(const std::string& initial, const std::string& final) const;

};