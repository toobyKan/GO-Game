#ifndef NETWORK_CODEC_H
#define NETWORK_CODEC_H

#include "network_messages.hpp"
#include "wire_message.hpp"

class NetworkCodec {
public:
    static bool encode(
        const network::MoveRequest& request,
        WireMessage& output
    );

    static bool decode(
        const WireMessage& message,
        network::MoveRequest& output
    );

    static bool encode(
        const network::MoveAccepted& message,
        WireMessage& output
    );

    static bool decode(
        const WireMessage& message,
        network::MoveAccepted& output
    );

    static bool encode(
        const network::MoveRejected& message,
        WireMessage& output
    );

    static bool decode(
        const WireMessage& message,
        network::MoveRejected& output
    );

    static bool encode(
        const network::MatchFound& message,
        WireMessage& output
    );

    static bool decode(
        const WireMessage& message,
        network::MatchFound& output
    );

    static bool encode(
        const network::BoardSnapshot& snapshot,
        WireMessage& output
    );

    static bool decode(
        const WireMessage& message,
        network::BoardSnapshot& output
    );

    static bool serialize(
        const WireMessage& message,
        std::vector<std::uint8_t>& output
    );

    static bool deserialize(
        const std::vector<std::uint8_t>& bytes,
        WireMessage& output
    );
};


#endif