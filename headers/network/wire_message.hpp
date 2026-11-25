#ifndef WIRE_MESSAGE_H
#define WIRE_MESSAGE_H

#include "protocol_types.hpp"

#include <cstdint>
#include <vector>

struct WireMessage {
    network::ProtocolMessageType type;
    std::vector<std::uint8_t> payload;
};

#endif