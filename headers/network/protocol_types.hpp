#ifndef PROTOCOL_TYPES_H
#define PROTOCOL_TYPES_H

#include <cstdint>

namespace network{

enum class ProtocolMessageType : std::uint16_t {
    FindMatchRequest = 0,
    CancelMatchRequest = 1,
    MatchFound = 2,
    MoveRequest = 3,
    MoveAccepted = 4,
    MoveRejected = 5,
    OpponentDisconnected = 6,
    GameOver = 7,
    Error = 8,
    BoardSnapshot = 9
};

enum class MoveRejectReason : std::uint8_t {
    OutOfBounds,
    Occupied,
    Suicide,
    Ko,
    NotYourTurn,
    InvalidSequence
};

}

#endif