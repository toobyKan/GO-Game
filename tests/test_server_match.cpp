#include "server/server_match.hpp"
#include "network/fake_network_client.hpp"
#include "network/network_codec.hpp"
#include "online_game_session.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace {

int failures = 0;

void expect(bool condition, const std::string& description) {
    if (!condition) {
        ++failures;
        std::cerr << "FAIL: " << description << '\n';
    }
}

void deliver(
    const std::vector<OutgoingMessage>& responses,
    FakeNetworkClient& blackClient,
    FakeNetworkClient& whiteClient
) {
    for (const OutgoingMessage& response : responses) {
        if (response.target == MatchPlayer::Black) {
            blackClient.injectIncoming(response.message);
        } else {
            whiteClient.injectIncoming(response.message);
        }
    }
}

std::vector<WireMessage> takeSingleOutgoing(
    FakeNetworkClient& client
) {
    std::vector<WireMessage> messages = client.takeOutgoing();
    expect(messages.size() == 1,
           "client emits exactly one request");
    return messages;
}

void testAcceptedMoveReachesBothClients() {
    FakeNetworkClient blackClient;
    FakeNetworkClient whiteClient;
    blackClient.connect();
    whiteClient.connect();

    OnlineGameSession black(3, blackClient);
    OnlineGameSession white(3, whiteClient);
    ServerMatch match(0, 3);

    expect(black.submitMove(1, 1).status == MoveStatus::Pending,
           "black submits a pending move");

    const std::vector<WireMessage> request =
        takeSingleOutgoing(blackClient);

    const std::vector<OutgoingMessage> responses =
        match.handleMessage(MatchPlayer::Black, request[0]);

    expect(responses.size() == 2,
           "accepted move is sent to both players");

    if (responses.size() == 2) {
        expect(
            responses[0].message.type ==
                network::ProtocolMessageType::BoardSnapshot,
            "accepted move produces a board snapshot"
        );
        expect(
            responses[1].message.type ==
                network::ProtocolMessageType::BoardSnapshot,
            "the same snapshot is sent to the other player"
        );

        network::BoardSnapshot snapshot;
        expect(
            NetworkCodec::decode(responses[0].message, snapshot),
            "server snapshot decodes"
        );
        expect(snapshot.stateVersion == 1,
               "first accepted move creates state version one");
        expect(snapshot.stones.size() == 9,
               "snapshot contains the complete board");
    }

    deliver(responses, blackClient, whiteClient);

    black.update(0.016);
    white.update(0.016);

    expect(match.getBoard().getStoneAt(1, 1) == Stone::Black,
           "server board contains the accepted black stone");
    expect(black.getBoard().getStoneAt(1, 1) == Stone::Black,
           "black client receives the accepted move");
    expect(white.getBoard().getStoneAt(1, 1) == Stone::Black,
           "white client receives the accepted move");
}

void testWrongTurnIsRejectedAndSequenceIsConsumed() {
    FakeNetworkClient blackClient;
    FakeNetworkClient whiteClient;
    blackClient.connect();
    whiteClient.connect();

    OnlineGameSession black(3, blackClient);
    OnlineGameSession white(3, whiteClient);
    ServerMatch match(0, 3);

    black.submitMove(0, 0);
    const std::vector<WireMessage> blackRequest =
        takeSingleOutgoing(blackClient);
    deliver(
        match.handleMessage(MatchPlayer::Black, blackRequest[0]),
        blackClient,
        whiteClient
    );
    black.update(0.016);
    white.update(0.016);

    // Black's next request has the correct sequence, but it is not Black's turn.
    expect(black.submitMove(2, 2).status == MoveStatus::Pending,
           "black can submit a request while the server decides the turn");
    const std::vector<WireMessage> wrongTurnRequest =
        takeSingleOutgoing(blackClient);
    const std::vector<OutgoingMessage> rejected =
        match.handleMessage(
            MatchPlayer::Black,
            wrongTurnRequest[0]
        );

    expect(rejected.size() == 1,
           "wrong-turn request is sent only to its sender");
    deliver(rejected, blackClient, whiteClient);
    black.update(0.016);

    network::MoveRejected rejection;
    expect(NetworkCodec::decode(rejected[0].message, rejection),
           "wrong-turn response decodes");
    expect(
        rejection.reason == network::MoveRejectReason::NotYourTurn,
        "wrong-turn response has the correct reason"
    );

    // White's first request remains sequence one because it has not moved yet.
    expect(white.submitMove(2, 2).status == MoveStatus::Pending,
           "white can move after black is rejected");
    const std::vector<WireMessage> whiteRequest =
        takeSingleOutgoing(whiteClient);
    const std::vector<OutgoingMessage> accepted =
        match.handleMessage(MatchPlayer::White, whiteRequest[0]);

    expect(accepted.size() == 2,
           "white's legal move is broadcast to both clients");
}

void testInvalidSequenceIsRejectedWithoutChangingBoard() {
    FakeNetworkClient blackClient;
    blackClient.connect();

    ServerMatch match(0, 3);

    network::MoveRequest request;
    request.matchId = 0;
    request.sequenceNumber = 99;
    request.x = 1;
    request.y = 1;

    WireMessage message;
    NetworkCodec::encode(request, message);

    const std::vector<OutgoingMessage> responses =
        match.handleMessage(MatchPlayer::Black, message);

    expect(responses.size() == 1,
           "invalid sequence produces one rejection");
    expect(match.getBoard().getStoneAt(1, 1) == Stone::None,
           "invalid sequence does not change the server board");

    network::MoveRejected rejection;
    expect(NetworkCodec::decode(responses[0].message, rejection),
           "invalid-sequence response decodes");
    expect(
        rejection.reason == network::MoveRejectReason::InvalidSequence,
        "invalid-sequence response has the correct reason"
    );
}

void testIllegalMoveIsRejectedWithoutChangingBoard() {
    FakeNetworkClient blackClient;
    blackClient.connect();

    ServerMatch match(0, 3);

    network::MoveRequest first;
    first.matchId = 0;
    first.sequenceNumber = 1;
    first.x = 1;
    first.y = 1;

    WireMessage firstMessage;
    NetworkCodec::encode(first, firstMessage);
    match.handleMessage(MatchPlayer::Black, firstMessage);

    network::MoveRequest second;
    second.matchId = 0;
    second.sequenceNumber = 1;
    second.x = 1;
    second.y = 1;

    WireMessage secondMessage;
    NetworkCodec::encode(second, secondMessage);

    const std::vector<OutgoingMessage> responses =
        match.handleMessage(MatchPlayer::White, secondMessage);

    expect(responses.size() == 1,
           "illegal move produces one rejection");
    expect(match.getBoard().getStoneAt(1, 1) == Stone::Black,
           "illegal move does not overwrite the existing stone");

    network::MoveRejected rejection;
    expect(NetworkCodec::decode(responses[0].message, rejection),
           "illegal-move response decodes");
    expect(
        rejection.reason == network::MoveRejectReason::Occupied,
        "illegal-move response has the correct reason"
    );
}

} // namespace

int main() {
    testAcceptedMoveReachesBothClients();
    testWrongTurnIsRejectedAndSequenceIsConsumed();
    testInvalidSequenceIsRejectedWithoutChangingBoard();
    testIllegalMoveIsRejectedWithoutChangingBoard();

    if (failures != 0) {
        std::cerr << failures << " test(s) failed\n";
        return 1;
    }

    std::cout << "All server-match integration tests passed\n";
    return 0;
}
