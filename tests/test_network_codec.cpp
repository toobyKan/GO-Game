#include "network/network_codec.hpp"

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace {

int failures = 0;

void expect(bool condition, const std::string& description)
{
    if (!condition) {
        ++failures;
        std::cerr << "FAIL: " << description << '\n';
    }
}

void testMoveRequestRoundTrip()
{
    network::MoveRequest original;
    original.matchId = 42;
    original.sequenceNumber = 7;
    original.x = 3;
    original.y = 11;

    WireMessage encoded;
    expect(NetworkCodec::encode(original, encoded),
           "move request encodes");

    std::vector<std::uint8_t> bytes;
    expect(NetworkCodec::serialize(encoded, bytes),
           "move request serializes");

    WireMessage decodedMessage;
    expect(NetworkCodec::deserialize(bytes, decodedMessage),
           "move request deserializes");

    network::MoveRequest decoded;
    expect(NetworkCodec::decode(decodedMessage, decoded),
           "move request decodes");

    expect(decoded.matchId == original.matchId,
           "move request preserves match ID");
    expect(decoded.sequenceNumber == original.sequenceNumber,
           "move request preserves sequence number");
    expect(decoded.x == original.x,
           "move request preserves x coordinate");
    expect(decoded.y == original.y,
           "move request preserves y coordinate");
}

void testMoveAcceptedRoundTrip()
{
    network::MoveAccepted original;
    original.matchId = 100;
    original.sequenceNumber = 12;
    original.x = 4;
    original.y = 8;
    original.player = 1;

    WireMessage encoded;
    expect(NetworkCodec::encode(original, encoded),
           "accepted move encodes");

    std::vector<std::uint8_t> bytes;
    expect(NetworkCodec::serialize(encoded, bytes),
           "accepted move serializes");

    WireMessage decodedMessage;
    expect(NetworkCodec::deserialize(bytes, decodedMessage),
           "accepted move deserializes");

    network::MoveAccepted decoded;
    expect(NetworkCodec::decode(decodedMessage, decoded),
           "accepted move decodes");

    expect(decoded.matchId == original.matchId,
           "accepted move preserves match ID");
    expect(decoded.sequenceNumber == original.sequenceNumber,
           "accepted move preserves sequence number");
    expect(decoded.x == original.x,
           "accepted move preserves x coordinate");
    expect(decoded.y == original.y,
           "accepted move preserves y coordinate");
    expect(decoded.player == original.player,
           "accepted move preserves player");
}

void testMoveRejectedRoundTrip()
{
    network::MoveRejected original;
    original.matchId = 100;
    original.sequenceNumber = 13;
    original.reason = network::MoveRejectReason::Occupied;

    WireMessage encoded;
    expect(NetworkCodec::encode(original, encoded),
           "rejected move encodes");

    std::vector<std::uint8_t> bytes;
    expect(NetworkCodec::serialize(encoded, bytes),
           "rejected move serializes");

    WireMessage decodedMessage;
    expect(NetworkCodec::deserialize(bytes, decodedMessage),
           "rejected move deserializes");

    network::MoveRejected decoded;
    expect(NetworkCodec::decode(decodedMessage, decoded),
           "rejected move decodes");

    expect(decoded.matchId == original.matchId,
           "rejected move preserves match ID");
    expect(decoded.sequenceNumber == original.sequenceNumber,
           "rejected move preserves sequence number");
    expect(decoded.reason == original.reason,
           "rejected move preserves reason");
}

void testMatchFoundRoundTrip()
{
    network::MatchFound original;
    original.matchId = 0x01020304;
    original.boardSize = 19;
    original.localPlayer = 1;
    original.startingPlayer = 2;

    WireMessage encoded;
    expect(NetworkCodec::encode(original, encoded),
           "match-found message encodes");

    std::vector<std::uint8_t> bytes;
    expect(NetworkCodec::serialize(encoded, bytes),
           "match-found message serializes");

    WireMessage decodedMessage;
    expect(NetworkCodec::deserialize(bytes, decodedMessage),
           "match-found message deserializes");

    network::MatchFound decoded;
    expect(NetworkCodec::decode(decodedMessage, decoded),
           "match-found message decodes");

    expect(decoded.matchId == original.matchId,
           "match-found preserves match ID");
    expect(decoded.boardSize == original.boardSize,
           "match-found preserves board size");
    expect(decoded.localPlayer == original.localPlayer,
           "match-found preserves local player");
    expect(decoded.startingPlayer == original.startingPlayer,
           "match-found preserves starting player");
}

void testBoardSnapshotRoundTrip()
{
    network::BoardSnapshot original;
    original.matchId = 42;
    original.stateVersion = 17;
    original.boardSize = 3;
    original.currentPlayer = 2;
    original.stones = {
        0, 1, 0,
        0, 2, 0,
        1, 0, 0
    };

    WireMessage encoded;
    expect(NetworkCodec::encode(original, encoded),
           "board snapshot encodes");

    std::vector<std::uint8_t> bytes;
    expect(NetworkCodec::serialize(encoded, bytes),
           "board snapshot serializes");

    WireMessage decodedMessage;
    expect(NetworkCodec::deserialize(bytes, decodedMessage),
           "board snapshot deserializes");

    network::BoardSnapshot decoded;
    expect(NetworkCodec::decode(decodedMessage, decoded),
           "board snapshot decodes");

    expect(decoded.matchId == original.matchId,
           "board snapshot preserves match ID");
    expect(decoded.stateVersion == original.stateVersion,
           "board snapshot preserves state version");
    expect(decoded.boardSize == original.boardSize,
           "board snapshot preserves board size");
    expect(decoded.currentPlayer == original.currentPlayer,
           "board snapshot preserves current player");
    expect(decoded.stones == original.stones,
           "board snapshot preserves all board cells");
}

void testMatchFoundRejectsInvalidPayload()
{
    WireMessage message;
    message.type = network::ProtocolMessageType::MatchFound;

    network::MatchFound output;

    message.payload = {0, 0, 0, 1, 19, 1};
    expect(!NetworkCodec::decode(message, output),
           "match-found rejects a truncated payload");

    message.payload = {0, 0, 0, 1, 0, 1, 2};
    expect(!NetworkCodec::decode(message, output),
           "match-found rejects a zero board size");

    message.payload = {0, 0, 0, 1, 19, 3, 1};
    expect(!NetworkCodec::decode(message, output),
           "match-found rejects an invalid local player");

    message.payload = {0, 0, 0, 1, 19, 1, 3};
    expect(!NetworkCodec::decode(message, output),
           "match-found rejects an invalid starting player");
}

void testBoardSnapshotRejectsInvalidPayload()
{
    WireMessage message;
    message.type = network::ProtocolMessageType::BoardSnapshot;

    network::BoardSnapshot output;

    message.payload = {
        0, 0, 0, 1,
        0, 0, 0, 2,
        3, 2,
        0, 1
    };
    expect(!NetworkCodec::decode(message, output),
           "board snapshot rejects an incomplete cell array");

    message.payload = {
        0, 0, 0, 1,
        0, 0, 0, 2,
        3, 3,
        0, 1, 0, 0, 2, 0, 1, 0, 0
    };
    expect(!NetworkCodec::decode(message, output),
           "board snapshot rejects an invalid current player");

    message.payload = {
        0, 0, 0, 1,
        0, 0, 0, 2,
        3, 2,
        0, 1, 0, 0, 2, 0, 4, 0, 0
    };
    expect(!NetworkCodec::decode(message, output),
           "board snapshot rejects an invalid stone value");
}

void testBoardSnapshotRejectsInvalidEncodeInput()
{
    network::BoardSnapshot snapshot;
    snapshot.matchId = 1;
    snapshot.stateVersion = 1;
    snapshot.boardSize = 3;
    snapshot.currentPlayer = 1;

    WireMessage output;

    snapshot.stones = {0, 1};
    expect(!NetworkCodec::encode(snapshot, output),
           "snapshot encoding rejects the wrong cell count");

    snapshot.stones = {
        0, 1, 0,
        0, 2, 0,
        1, 0, 3
    };
    expect(!NetworkCodec::encode(snapshot, output),
           "snapshot encoding rejects an invalid stone value");

    snapshot.stones.assign(32 * 32, 0);
    snapshot.boardSize = 32;
    expect(!NetworkCodec::encode(snapshot, output),
           "snapshot encoding rejects a payload over the size limit");
}

void testSerializedHeader()
{
    network::MoveRequest request;
    request.matchId = 0x01020304;
    request.sequenceNumber = 0x05060708;
    request.x = 0x090a;
    request.y = 0x0b0c;

    WireMessage message;
    expect(NetworkCodec::encode(request, message),
           "header test request encodes");

    std::vector<std::uint8_t> bytes;
    expect(NetworkCodec::serialize(message, bytes),
           "header test request serializes");

    expect(bytes.size() == 19,
           "serialized move request has the expected length");
    if (bytes.size() < 11) {
        return;
    }

    expect(bytes[0] == 1,
           "serialized header contains protocol version");
    expect(bytes[1] == 0,
           "serialized header stores the high type byte");
    expect(bytes[2] == static_cast<std::uint8_t>(
               static_cast<std::uint16_t>(
                   network::ProtocolMessageType::MoveRequest)),
           "serialized header contains message type");

    expect(bytes[3] == 0 && bytes[4] == 0 &&
               bytes[5] == 0 && bytes[6] == 12,
           "serialized header contains payload length");

    expect(bytes[7] == 0x01 && bytes[8] == 0x02 &&
               bytes[9] == 0x03 && bytes[10] == 0x04,
           "serialized payload uses big-endian match ID");
}

void testDeserializeRejectsMalformedInput()
{
    WireMessage output;

    expect(!NetworkCodec::deserialize({}, output),
           "empty input is rejected");

    std::vector<std::uint8_t> wrongVersion = {
        2, 0, 3, 0, 0, 0, 0
    };
    expect(!NetworkCodec::deserialize(wrongVersion, output),
           "unsupported protocol version is rejected");

    std::vector<std::uint8_t> unknownType = {
        1, 0xff, 0xff, 0, 0, 0, 0
    };
    expect(!NetworkCodec::deserialize(unknownType, output),
           "unknown message type is rejected");

    std::vector<std::uint8_t> truncatedPayload = {
        1, 0, 4, 0, 0, 0, 12, 0, 1
    };
    expect(!NetworkCodec::deserialize(truncatedPayload, output),
           "truncated payload is rejected");
}

void testDecodeRejectsInvalidMoveRejectedReason()
{
    WireMessage message;
    message.type = network::ProtocolMessageType::MoveRejected;
    message.payload = {
        0, 0, 0, 1,
        0, 0, 0, 2,
        0xff
    };

    network::MoveRejected output;
    expect(!NetworkCodec::decode(message, output),
           "invalid move-rejected reason is rejected");
}

} // namespace

int main()
{
    testMoveRequestRoundTrip();
    testMoveAcceptedRoundTrip();
    testMoveRejectedRoundTrip();
    testMatchFoundRoundTrip();
    testBoardSnapshotRoundTrip();
    testSerializedHeader();
    testDeserializeRejectsMalformedInput();
    testDecodeRejectsInvalidMoveRejectedReason();
    testMatchFoundRejectsInvalidPayload();
    testBoardSnapshotRejectsInvalidPayload();
    testBoardSnapshotRejectsInvalidEncodeInput();

    if (failures != 0) {
        std::cerr << failures << " test(s) failed\n";
        return 1;
    }

    std::cout << "All network codec tests passed\n";
    return 0;
}
