#ifndef GAME_SESSION_H
#define GAME_SESSION_H

#include "board.hpp"

class IGameSession {
public:
    virtual ~IGameSession() = default;

    virtual bool submitMove(int x, int y) = 0;
    virtual const Board& getBoard() const = 0;
    virtual Stone getCurrentPlayer() const = 0;
    virtual void update(double deltaTime) = 0;
};

#endif
