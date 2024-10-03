#ifndef PLAYER_H
#define PLAYER_H

#include "stone.hpp"
#include "board.hpp"

class Player {
public:
    Player(Color);

    Color getColor() const;

    bool makeMove(Board&, int, int);

private:
    Color color;

};

#endif