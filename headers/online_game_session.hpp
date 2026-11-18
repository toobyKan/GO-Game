#ifndef ONLINE_GAME_SESSION_H
#define ONLINE_GAME_SESSION_H

#include "game_session.hpp"
#include "board.hpp"
#include "game_logic.hpp"


class OnlineGameSession : public IGameSession {
public:
    OnlineGameSession(int board_size);

    bool submitMove(int x, int y) override;

    const Board& getBoard() const override;
    Stone getCurrentPlayer() const override;
    
    void update(double deltaTime) override;

private:
    Board board_;
    GameLogic gameLogic_;
}; 

#endif