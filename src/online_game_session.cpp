#include "online_game_session.hpp"

#include "network/network_codec.hpp"
#include "network/network_messages.hpp"

OnlineGameSession::OnlineGameSession(int board_size, INetworkClient& client) : board_(board_size), client_(client) {}

MoveResult OnlineGameSession::submitMove(int x, int y) {
    if (x < 0 || x >= board_.getSize() ||
        y < 0 || y >= board_.getSize()) {
        return MoveResult{MoveStatus::OutOfBounds};
    }

    if (movePending_) {
        return MoveResult{MoveStatus::Pending};
    }

    network::MoveRequest request;
    request.matchId = matchId_;
    request.sequenceNumber = nextSequenceNumber_;
    request.x = static_cast<std::int16_t>(x);
    request.y = static_cast<std::int16_t>(y);

    WireMessage message;
    if (!NetworkCodec::encode(request, message)) {
        return MoveResult{MoveStatus::OutOfBounds};
    }

    if (!client_.sendMessage(message)) {
        return MoveResult{MoveStatus::NetworkError};
    }

    pendingSequenceNumber_ = request.sequenceNumber;
    ++nextSequenceNumber_;
    movePending_ = true;

    return MoveResult{MoveStatus::Pending};
}

const Board& OnlineGameSession::getBoard() const {
    return board_;
}

Stone OnlineGameSession::getCurrentPlayer() const {
    return currentPlayer_;
}

void OnlineGameSession::update(double deltaTime) {
    client_.update(deltaTime);

    const std::vector<WireMessage> messages = client_.pollMessages();
    for (const WireMessage& message : messages) {
        handleMessage(message);
    }
}

void OnlineGameSession::handleMessage(const WireMessage& message) {
    switch (message.type) {
    case network::ProtocolMessageType::MoveAccepted:
        handleMoveAccepted(message);
        break;

    case network::ProtocolMessageType::MoveRejected:
        handleMoveRejected(message);
        break;

    case network::ProtocolMessageType::BoardSnapshot:
        handleBoardSnapshot(message);
        break;

    default:
        break;
    }
}

void OnlineGameSession::handleMoveAccepted(const WireMessage& message) {
    network::MoveAccepted acceptedMove;

    if (!NetworkCodec::decode(message, acceptedMove) ||
        acceptedMove.matchId != matchId_) {
        return;
    }

    if (movePending_ &&
        acceptedMove.sequenceNumber == pendingSequenceNumber_) {
        movePending_ = false;
    }

    const Stone player = acceptedMove.player == 1 ? Stone::Black : Stone::White;

    board_.setStoneAt(acceptedMove.x, acceptedMove.y, player);
    currentPlayer_ = (
        player == Stone::Black ? Stone::White : Stone::Black
    );
}

void OnlineGameSession::handleMoveRejected(const WireMessage& message) {
    network::MoveRejected rejectedMove;

    if (!NetworkCodec::decode(message, rejectedMove) ||
        rejectedMove.matchId != matchId_) {
        return;
    }

    if (movePending_ &&
        rejectedMove.sequenceNumber == pendingSequenceNumber_) {
        movePending_ = false;
    }
}

void OnlineGameSession::handleBoardSnapshot(
    const WireMessage& message
) {
    network::BoardSnapshot snapshot;

    if (!NetworkCodec::decode(message, snapshot) ||
        snapshot.matchId != matchId_ ||
        snapshot.boardSize != board_.getSize() ||
        snapshot.stateVersion < stateVersion_) {
        return;
    }

    std::vector<Stone> stones;
    stones.reserve(snapshot.stones.size());

    for (std::uint8_t encodedStone : snapshot.stones) {
        if (encodedStone > 2) {
            return;
        }

        stones.push_back(static_cast<Stone>(encodedStone));
    }

    board_.restoreState(stones);
    stateVersion_ = snapshot.stateVersion;
    currentPlayer_ = static_cast<Stone>(snapshot.currentPlayer);
    movePending_ = false;
}
