#ifndef NETWORK_MESSAGES_H
#define NETWORK_MESSAGES_H

#include "protocol_types.hpp"

#include <cstdint>
#include <vector>

namespace network {

struct FindMatchRequest {
};

struct CancelMatchRequest {
};

struct MatchFound {
    std::uint32_t matchId;
    std::uint8_t boardSize;
    std::uint8_t localPlayer;
    std::uint8_t startingPlayer;
};

struct MoveRequest {
    std::uint32_t matchId;
    std::uint32_t sequenceNumber;
    std::int16_t x;
    std::int16_t y;
};

struct MoveAccepted {
    std::uint32_t matchId;
    std::uint32_t sequenceNumber;
    std::int16_t x;
    std::int16_t y;
    std::uint8_t player;
};

struct MoveRejected {
    std::uint32_t matchId;
    std::uint32_t sequenceNumber;
    MoveRejectReason reason;
};

struct BoardSnapshot {
    std::uint32_t matchId;
    std::uint32_t stateVersion;
    std::uint8_t boardSize;
    std::uint8_t currentPlayer;
    std::vector<std::uint8_t> stones;
};

struct OpponentDisconnected {
    std::uint32_t matchId;
};

struct GameOver {
    std::uint32_t matchId;
    std::uint8_t winner;
};

}

#endif