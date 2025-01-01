#pragma once
#include "board.h"
#include <string>

class Game {
public:
    Game() : turn(Color::White), gameOver(false), board() {};
    void play();

    

private:
    Board board;
    bool gameOver;
    bool turn; // 1 is white, 0 is black

    void handlePawnPromotion(const Position& p);
    Position parsePosition(const std::string& s) const;
    bool isValidInput(const std::string& initial, const std::string& final) const;

};