#ifndef SERVER_MATCH_H
#define SERVER_MATCH_H

#include "network/wire_message.hpp"
#include "network/network_messages.hpp"
#include "board.hpp"
#include "game_logic.hpp"

#include <vector>

enum class MatchPlayer {
    Black,
    White
};

struct OutgoingMessage {
    MatchPlayer target;
    WireMessage message;
};

class ServerMatch {
public:
    ServerMatch(std::uint32_t matchId, int boardSize);

    std::vector<OutgoingMessage> handleMessage(MatchPlayer sender, const WireMessage& message);

    const Board& getBoard() const;

private:
    std::uint32_t& expectedSequence(MatchPlayer player);

    OutgoingMessage makeRejected(MatchPlayer recipient, const network::MoveRequest& request, network::MoveRejectReason reason) const;
    
    network::MoveRejectReason convertReason(MoveStatus status) const;
    network::BoardSnapshot makeBoardSnapshot() const;

    Board board_;
    GameLogic gameLogic_;

    std::uint32_t matchId_;
    std::uint32_t stateVersion_ = 0;
    std::uint32_t expectedBlackSequence_ = 1;
    std::uint32_t expectedWhiteSequence_ = 1;

};

#endif
