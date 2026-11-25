#include "network/network_codec.hpp"
#include "network/protocol_types.hpp"


namespace {
    const std::uint8_t ProtocolVersion = 1;
    const std::size_t HeaderSize = 1 + 2 + 4;
    const std::uint32_t MaxPayloadSize = 1024;

    void appendUint16(std::vector<std::uint8_t>& output, std::uint16_t value) {
        output.push_back(static_cast<std::uint8_t>((value >> 8) & 0xff));
        output.push_back(static_cast<std::uint8_t>(value & 0xff));

    }

    void appendUint32(std::vector<std::uint8_t>& output, std::uint32_t value) {
        output.push_back(static_cast<std::uint8_t>((value >> 24) & 0xff));
        output.push_back(static_cast<std::uint8_t>((value >> 16) & 0xff));
        output.push_back(static_cast<std::uint8_t>((value >> 8) & 0xff));
        output.push_back(static_cast<std::uint8_t>(value & 0xff));

    }

    bool isKnownProtocolMessageType(std::uint16_t value) {
        switch (static_cast<network::ProtocolMessageType>(value)) {
            case network::ProtocolMessageType::FindMatchRequest:
            case network::ProtocolMessageType::CancelMatchRequest:
            case network::ProtocolMessageType::MatchFound:
            case network::ProtocolMessageType::MoveRequest:
            case network::ProtocolMessageType::MoveAccepted:
            case network::ProtocolMessageType::MoveRejected:
            case network::ProtocolMessageType::OpponentDisconnected:
            case network::ProtocolMessageType::BoardSnapshot:
            case network::ProtocolMessageType::GameOver:
            case network::ProtocolMessageType::Error:
                return true;
        }

        return false;
    }

    bool isKnownMoveRejectReason(std::uint8_t value) {
        switch (static_cast<network::MoveRejectReason>(value)) {
            case network::MoveRejectReason::OutOfBounds:
            case network::MoveRejectReason::Occupied:
            case network::MoveRejectReason::Suicide:
            case network::MoveRejectReason::Ko:
            case network::MoveRejectReason::NotYourTurn:
            case network::MoveRejectReason::InvalidSequence:
                return true;
        }

        return false;
    }


    bool readUint16(const std::vector<std::uint8_t>& bytes, std::size_t& offset, std::uint16_t& output) {
        if (offset > bytes.size() || bytes.size() - offset < 2) return false;
        
        // assembles back what appendUint16 did
        output = ((static_cast<uint16_t>(bytes[offset]) << 8) | static_cast<uint16_t>(bytes[offset + 1]));

        offset += 2;

        return true;
    }

    bool readUint32(const std::vector<std::uint8_t>& bytes, std::size_t& offset, std::uint32_t& output) {
        if (offset > bytes.size() || bytes.size() - offset < 4) return false;
    
        
        // assembles back what appendUint32 did
        output = (
            (static_cast<uint32_t>(bytes[offset]) << 24) | 
            (static_cast<uint32_t>(bytes[offset + 1]) << 16) | 
            (static_cast<uint32_t>(bytes[offset + 2]) << 8) | 
            static_cast<uint16_t>(bytes[offset + 3])
        );

        offset += 4;

        return true;
    }
}



bool NetworkCodec::serialize(const WireMessage& message, std::vector<std::uint8_t>& output) {
    //output = [protocol_version] + [message_type] + [payload_size] + [payload]

    if (message.payload.size() > MaxPayloadSize) return false;

    output.clear();
    output.reserve(HeaderSize + message.payload.size());

    output.push_back(ProtocolVersion);

    appendUint16(output, static_cast<uint16_t>(message.type));
    appendUint32(output, static_cast<uint32_t>(message.payload.size()));

    output.insert(output.end(), message.payload.begin(), message.payload.end());

    return true;
}

bool NetworkCodec::deserialize(const std::vector<std::uint8_t>& bytes, WireMessage& output) {
    if (bytes.size() < HeaderSize) return false;

    std::size_t offset = 0;

    const std::uint8_t protocolVersion = bytes[offset++];
    if (protocolVersion != ProtocolVersion) return false;

    std::uint16_t rawType = 0;
    if (!readUint16(bytes, offset, rawType)) return false;
    if (!isKnownProtocolMessageType(rawType)) return false;

    std::uint32_t payloadSize = 0;
    if (!readUint32(bytes, offset, payloadSize)) return false;
    if (payloadSize > MaxPayloadSize) return false;
    if (bytes.size() - offset != payloadSize) return false;

    output.type = static_cast<network::ProtocolMessageType>(rawType);
    output.payload.assign(bytes.begin() + offset, bytes.end());

    return true;
}

bool NetworkCodec::encode(const network::MoveRequest& request, WireMessage& output) {
    output.type = network::ProtocolMessageType::MoveRequest;
    output.payload.clear();

    appendUint32(output.payload, request.matchId);
    appendUint32(output.payload, request.sequenceNumber);
    appendUint16(output.payload, static_cast<uint16_t>(request.x));
    appendUint16(output.payload, static_cast<uint16_t>(request.y));

    return true;
}

bool NetworkCodec::decode(const WireMessage& message, network::MoveRequest& output) {
    if (message.type != network::ProtocolMessageType::MoveRequest) return false;

    const std::size_t expectedSize = (
        sizeof(std::uint32_t) +
        sizeof(std::uint32_t) +
        sizeof(std::uint16_t) +
        sizeof(std::uint16_t)
    );

    if (message.payload.size() != expectedSize) return false;
    
    std::size_t offset = 0;

    if (!readUint32(message.payload, offset, output.matchId)) return false;
    if (!readUint32(message.payload, offset, output.sequenceNumber)) return false;
    
    std::uint16_t rawX = 0;
    std::uint16_t rawY = 0;
    if (!readUint16(message.payload, offset, rawX)) return false;
    if (!readUint16(message.payload, offset, rawY)) return false;

    output.x = static_cast<std::int16_t>(rawX);
    output.y = static_cast<std::int16_t>(rawY);

    return offset == message.payload.size();
}

bool NetworkCodec::encode(const network::MoveAccepted& request, WireMessage& output) {
    output.type = network::ProtocolMessageType::MoveAccepted;
    output.payload.clear();

    appendUint32(output.payload, request.matchId);
    appendUint32(output.payload, request.sequenceNumber);
    appendUint16(output.payload, static_cast<uint16_t>(request.x));
    appendUint16(output.payload, static_cast<uint16_t>(request.y));
    output.payload.push_back(request.player);

    return true;
}

bool NetworkCodec::decode(const WireMessage& message, network::MoveAccepted& output) {
    if (message.type != network::ProtocolMessageType::MoveAccepted) return false;

    const std::size_t expectedSize = (
        sizeof(std::uint32_t) +
        sizeof(std::uint32_t) +
        sizeof(std::uint16_t) +
        sizeof(std::uint16_t) +
        sizeof(std::uint8_t)
    );

    if (message.payload.size() != expectedSize) return false;
    
    std::size_t offset = 0;

    if (!readUint32(message.payload, offset, output.matchId)) return false;
    if (!readUint32(message.payload, offset, output.sequenceNumber)) return false;
    
    std::uint16_t rawX = 0;
    std::uint16_t rawY = 0;
    if (!readUint16(message.payload, offset, rawX)) return false;
    if (!readUint16(message.payload, offset, rawY)) return false;

    output.x = static_cast<std::int16_t>(rawX);
    output.y = static_cast<std::int16_t>(rawY);

    if (offset >= message.payload.size()) return false;

    output.player = message.payload[offset++];

    return offset == message.payload.size();
}

bool NetworkCodec::encode(const network::MoveRejected& request, WireMessage& output) {
    output.type = network::ProtocolMessageType::MoveRejected;
    output.payload.clear();

    appendUint32(output.payload, request.matchId);
    appendUint32(output.payload, request.sequenceNumber);
    output.payload.push_back(static_cast<uint8_t>(request.reason));

    return true;
}

bool NetworkCodec::decode(const WireMessage& message, network::MoveRejected& output) {
    if (message.type != network::ProtocolMessageType::MoveRejected) return false;

    const std::size_t expectedSize = (
        sizeof(std::uint32_t) +
        sizeof(std::uint32_t) +
        sizeof(std::uint8_t)
    );

    if (message.payload.size() != expectedSize) return false;
    
    std::size_t offset = 0;

    if (!readUint32(message.payload, offset, output.matchId)) return false;
    if (!readUint32(message.payload, offset, output.sequenceNumber)) return false;

    if (offset >= message.payload.size()) return false;

    const std::uint8_t rawReason = message.payload[offset++];
    if (!isKnownMoveRejectReason(rawReason)) return false;

    output.reason = static_cast<network::MoveRejectReason>(rawReason);

    return offset == message.payload.size();
}

bool NetworkCodec::encode(const network::MatchFound& request, WireMessage& output) {
    output.type = network::ProtocolMessageType::MatchFound;
    output.payload.clear();

    appendUint32(output.payload, request.matchId);
    output.payload.push_back(request.boardSize);
    output.payload.push_back(request.localPlayer);
    output.payload.push_back(request.startingPlayer);

    return true;
}

bool NetworkCodec::decode(const WireMessage& message, network::MatchFound& output) {
    if (message.type != network::ProtocolMessageType::MatchFound) return false;

    const std::size_t expectedSize = (
        sizeof(std::uint32_t) +
        sizeof(std::uint8_t) +
        sizeof(std::uint8_t) +
        sizeof(std::uint8_t)
    );

    if (message.payload.size() != expectedSize) return false;

    std::size_t offset = 0;

    if (!readUint32(message.payload, offset, output.matchId)) return false;

    if (offset >= message.payload.size()) return false;
    output.boardSize = message.payload[offset++];

    if (offset >= message.payload.size()) return false;
    output.localPlayer = message.payload[offset++];

    if (offset >= message.payload.size()) return false;

    output.startingPlayer = message.payload[offset++];

    if (output.boardSize == 0) return false;

    if (output.localPlayer > 2 || output.startingPlayer > 2) return false;

    return offset == message.payload.size();
}

bool NetworkCodec::encode(const network::BoardSnapshot& snapshot, WireMessage& output) {
    if (snapshot.boardSize == 0) return false;

    const std::size_t cellCount = (
        static_cast<std::size_t>(snapshot.boardSize) * static_cast<std::size_t>(snapshot.boardSize)
    );

    if (snapshot.stones.size() != cellCount) return false;

    if (snapshot.currentPlayer > 2) return false;

    output.type = network::ProtocolMessageType::BoardSnapshot;
    output.payload.clear();

    appendUint32(output.payload, snapshot.matchId);

    appendUint32(output.payload, snapshot.stateVersion);

    output.payload.push_back(snapshot.boardSize);
    output.payload.push_back(snapshot.currentPlayer);

    for (std::uint8_t stone : snapshot.stones) {
        if (stone > 2) {
            output.payload.clear();
            return false;
        }

        output.payload.push_back(stone);
    }

    return output.payload.size() <= MaxPayloadSize;
}

bool NetworkCodec::decode(const WireMessage& message, network::BoardSnapshot& output) {
    if (message.type != network::ProtocolMessageType::BoardSnapshot) return false;

    const std::size_t metadataSize =
        sizeof(std::uint32_t) +
        sizeof(std::uint32_t) +
        sizeof(std::uint8_t) +
        sizeof(std::uint8_t);

    if (message.payload.size() < metadataSize) return false;

    std::size_t offset = 0;

    if (!readUint32(message.payload, offset, output.matchId)) return false;
    if (!readUint32(message.payload, offset, output.stateVersion)) return false;

    output.boardSize = message.payload[offset++];
    output.currentPlayer = message.payload[offset++];

    if (output.boardSize == 0 || output.currentPlayer > 2) return false;

    const std::size_t cellCount = (
        static_cast<std::size_t>(output.boardSize) *
        static_cast<std::size_t>(output.boardSize)
    );

    if (message.payload.size() != metadataSize + cellCount) return false;

    output.stones.assign(message.payload.begin() + offset, message.payload.end());

    for (std::uint8_t stone : output.stones) {
        if (stone > 2) {
            output.stones.clear();
            return false;
        }
    }

    return true;
}
