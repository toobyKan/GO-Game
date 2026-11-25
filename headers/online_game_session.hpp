#ifndef ONLINE_GAME_SESSION_H
#define ONLINE_GAME_SESSION_H

#include "game_session.hpp"
#include "board.hpp"
#include "network/network_client.hpp"


class OnlineGameSession : public IGameSession {
public:
    OnlineGameSession(int board_size, INetworkClient& client);

    MoveResult submitMove(int x, int y) override;

    const Board& getBoard() const override;
    Stone getCurrentPlayer() const override;
    
    void update(double deltaTime) override;

private:
    void handleMessage(const WireMessage& message);
    void handleMoveAccepted(const WireMessage& message);
    void handleMoveRejected(const WireMessage& message);
    void handleBoardSnapshot(const WireMessage& message);

    Board board_;
    INetworkClient& client_;

    std::uint32_t matchId_ = 0;
    std::uint32_t stateVersion_ = 0;
    std::uint32_t nextSequenceNumber_ = 1;
    std::uint32_t pendingSequenceNumber_ = 0;
    Stone currentPlayer_ = Stone::Black;
    bool movePending_ = false;
}; 

#endif
