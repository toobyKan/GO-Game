#include <iostream>
#include "../includes/game.hpp"

class Game {
private:
    Board board;
    Player player1;
    Player player2;
    Player* currentPlayer;
    bool gameOver;

public:
    Game(int boardSize)
        : board(boardSize), player1(Color::Black), player2(Color::White), currentPlayer(&player1), gameOver(false) {}

    void switchTurn() {
        if (currentPlayer == &player1) {
            currentPlayer = &player2;
        } else {
            currentPlayer = &player1;
        }
    }

    void start() {
        while (!gameOver) {
            board.display();
            int x, y;
            std::cout << "Player " << (currentPlayer->getColor() == Color::Black ? "Black" : "White") << " turn. Enter row and column: ";
            std::cin >> x >> y;

            if (currentPlayer->makeMove(board, x, y)) {
                switchTurn();
            } else {
                std::cout << "Invalid move, try again." << std::endl;
            }

            // Add win conditions or termination logic
        }
    }
};
