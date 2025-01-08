#include "game.h"

void Game::play() {
    while (!gameOver){
        board.printBoard();

        std::string initial, final;
        bool validInput = false;
        while (!validInput){

            std::cout << "Input " << ((turn) ? "white move:" : "black move:") << std::endl;
            std::cin >> initial >> final;

            if (initial == "q" || initial == "Q" || initial == "quit" || initial == "Quit"){
                gameOver = 1;
                return;
            }

            if (isValidInput(initial, final)){
                validInput = true;
            }
            else {
                std::cout << "Invalid input.\n";
            }
        }

        // convert algebraic to matrix position
        Position i = parsePosition(initial);
        Position f = parsePosition(final);

        // move piece(s)
        board.movePiece(i, f, turn);

        // update turn
        turn = (turn) ? 0 : 1;

        // check for checkmate or draw
        if (board.isCheckmate(turn)){
            board.printBoard();
            std::cout << ((turn) ? "Black " : "White ") << "has won!\n";
            gameOver = 1;
        }
        if (board.isDraw(turn)){
            board.printBoard();
            std::cout << "Draw!\n";
            gameOver = 1;
        }
    }
}

bool Game::isValidInput(const std::string& initial, const std::string& final) const {
    if (initial.length() != 2 || final.length() != 2 || !isalpha(initial[0]) || !isdigit(initial[1]) || !isalpha(final[0]) || !isdigit(final[1])){
        return false;
    }

    Position i = parsePosition(initial);
    Position f = parsePosition(final);

    if (!isWithinBounds(i) || !isWithinBounds(f)){
        return false;
    }

    if (!board.getPiece(i) || board.getPiece(i)->getColor() != turn){
        return false;
    }

    if (!board.isValidMove(i, f, turn)){
        return false;
    }

    return true;
}

Position Game::parsePosition(const std::string& s) const {
    return Position{'8' - s[1], s[0] - 'a'};
}