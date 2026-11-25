#ifndef LOCAL_GAME_SESSION_H
#define LOCAL_GAME_SESSION_H

#include "game_session.hpp"
#include "board.hpp"
#include "game_logic.hpp"


class LocalGameSession : public IGameSession {
public:
    LocalGameSession(int board_size);

    MoveResult submitMove(int x, int y) override;

    const Board& getBoard() const override;
    Stone getCurrentPlayer() const override;

    void update(double deltaTime) override;

private:
    Board board_;
    GameLogic gameLogic_;
}; 

#endif