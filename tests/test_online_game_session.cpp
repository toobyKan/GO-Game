#include "online_game_session.hpp"
#include "network/fake_network_client.hpp"
#include "network/network_codec.hpp"

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

WireMessage makeAccepted(
    std::uint32_t sequence,
    std::int16_t x,
    std::int16_t y,
    std::uint8_t player
) {
    network::MoveAccepted accepted;
    accepted.matchId = 0;
    accepted.sequenceNumber = sequence;
    accepted.x = x;
    accepted.y = y;
    accepted.player = player;

    WireMessage message;
    NetworkCodec::encode(accepted, message);
    return message;
}

WireMessage makeRejected(
    std::uint32_t sequence,
    network::MoveRejectReason reason
) {
    network::MoveRejected rejected;
    rejected.matchId = 0;
    rejected.sequenceNumber = sequence;
    rejected.reason = reason;

    WireMessage message;
    NetworkCodec::encode(rejected, message);
    return message;
}

void test_connection_and_outgoing_request() {
    FakeNetworkClient client;
    OnlineGameSession session(3, client);

    expect(client.connect(), "fake client connects");
    expect(session.submitMove(1, 2).status == MoveStatus::Pending,
           "online move is pending after being sent");

    const std::vector<WireMessage> outgoing = client.takeOutgoing();
    expect(outgoing.size() == 1,
           "online session sends one move request");

    network::MoveRequest request;
    expect(NetworkCodec::decode(outgoing[0], request),
           "outgoing message is a move request");
    expect(request.sequenceNumber == 1,
           "first move uses sequence number one");
    expect(request.x == 1 && request.y == 2,
           "outgoing request preserves coordinates");
}

void test_pending_move_blocks_another_request() {
    FakeNetworkClient client;
    client.connect();
    OnlineGameSession session(3, client);

    expect(session.submitMove(0, 0).status == MoveStatus::Pending,
           "first move becomes pending");
    expect(session.submitMove(2, 2).status == MoveStatus::Pending,
           "second move remains pending while first is unresolved");

    expect(client.takeOutgoing().size() == 1,
           "only one request is sent while a move is pending");
}

void test_accepted_move_updates_replica() {
    FakeNetworkClient client;
    client.connect();
    OnlineGameSession session(3, client);

    expect(session.submitMove(1, 1).status == MoveStatus::Pending,
           "move is sent before acceptance");
    client.injectIncoming(makeAccepted(1, 1, 1, 1));
    session.update(0.016);

    expect(session.getBoard().getStoneAt(1, 1) == Stone::Black,
           "accepted move updates the client board");
    expect(session.getCurrentPlayer() == Stone::White,
           "accepted move updates the current player");

    expect(session.submitMove(0, 0).status == MoveStatus::Pending,
           "a new move can be sent after acceptance");
}

void test_rejected_move_leaves_replica_unchanged() {
    FakeNetworkClient client;
    client.connect();
    OnlineGameSession session(3, client);

    expect(session.submitMove(1, 1).status == MoveStatus::Pending,
           "move is sent before rejection");
    client.injectIncoming(makeRejected(
        1,
        network::MoveRejectReason::Occupied
    ));
    session.update(0.016);

    expect(session.getBoard().getStoneAt(1, 1) == Stone::None,
           "rejected move does not update the client board");
    expect(session.submitMove(0, 0).status == MoveStatus::Pending,
           "a new move can be sent after rejection");
}

void test_disconnected_client_reports_network_error() {
    FakeNetworkClient client;
    OnlineGameSession session(3, client);

    expect(session.submitMove(0, 0).status == MoveStatus::NetworkError,
           "sending while disconnected reports a network error");
}

void test_invalid_coordinate_is_rejected_locally() {
    FakeNetworkClient client;
    client.connect();
    OnlineGameSession session(3, client);

    expect(session.submitMove(-1, 0).status == MoveStatus::OutOfBounds,
           "negative coordinate is rejected locally");
    expect(client.takeOutgoing().empty(),
           "invalid coordinate is not sent to the network");
}

} // namespace

int main() {
    test_connection_and_outgoing_request();
    test_pending_move_blocks_another_request();
    test_accepted_move_updates_replica();
    test_rejected_move_leaves_replica_unchanged();
    test_disconnected_client_reports_network_error();
    test_invalid_coordinate_is_rejected_locally();

    if (failures != 0) {
        std::cerr << failures << " test(s) failed\n";
        return 1;
    }

    std::cout << "All online-session tests passed\n";
    return 0;
}
