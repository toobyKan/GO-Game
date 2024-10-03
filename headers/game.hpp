#ifndef GAME_H
#define GAME_H

#include "board.hpp"
#include "player.hpp"

class Game{
public:
    Game();

    void switchTurn();
    void start();

private:
    Board board;
    Player player1;
    Player player2;
    Player* currentPlayer;
    bool gameOver;
};

#endif