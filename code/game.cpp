#include "game.h"

void Game::run() {
    std::string opponent;
    bool validInput = false;
    while (!validInput){
        std::cout << "Choose opponent: (AI / P)\n";
        std::cin >> opponent;
        if (opponent == "AI"){
            validInput = 1;
            playEngine();
        }
        else if (opponent == "P"){
            validInput = 1;
            playPlayer();
        }
        else {
            std::cout << "Not an option!!\n";
        }
    }
}

void Game::playPlayer(){
    MoveInfo lastMove;
    while (!gameOver){
        board.printBoard();

        std::pair<Position, Position> move = getPlayerMove();

        if (move.first.col == -1 && move.first.row == -1){
            break;
        }

        // move piece(s)
        board.movePiece(move.first, move.second, turn);

        // update turn
        turn = !turn;

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

void Game::playEngine(){
    Engine engine;
    srand(time(0));
    bool player = (rand() % 2) ? turn : !turn;

    while (!gameOver){
        board.printBoard();
        std::pair<Position, Position> move;

        if (player == turn){ // player turn
            move = getPlayerMove();
            if (move.first.col == -1 && move.first.row == -1){break;}
        }
        else { // engine turn
            Board simulatedBoard(board);
            simulatedBoard.printBoard();
            move = engine.getBestMove(simulatedBoard, PLIES, turn);
        }

        // move piece(s)
        board.movePiece(move.first, move.second, turn);
    
        // update turn
        turn = !turn;

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

std::pair<Position, Position> Game::getPlayerMove(){
    std::string initial, final;
    bool validInput = false;
    while (!validInput){

        std::cout << "Input " << ((turn) ? "white move:" : "black move:\n");
        std::cin >> initial;
        if (initial == "Q" || initial == "q"){
            gameOver = 1;
            return std::pair<Position, Position> {{-1 , -1}, {-1, -1}};
        }
        std::cin >> final;
        if (final == "Q" || final == "q"){
            gameOver = 1;
            return std::pair<Position, Position> {{-1 , -1}, {-1, -1}};
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

    return std::pair<Position, Position>{i, f};
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

    if (!board.readPiece(i) || board.readPiece(i)->getColor() != turn){
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