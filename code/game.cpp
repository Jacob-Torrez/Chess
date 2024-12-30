#include "game.h"

void Game::play() {
    while (!gameOver){
        board.printBoard();

        std::string initial, final;
        std::cout << "Input " << ((turn) ? "white move:" : "black move:") << std::endl;
        std::cin >> initial >> final;

        Position i = parsePosition(initial);
        Position f = parsePosition(final);
    }
}

Position Game::parsePosition(const std::string& s) const {
    return Position{'8' - s[1], s[0] - 'a'};
}