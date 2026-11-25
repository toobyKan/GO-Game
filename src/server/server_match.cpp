#include "server/server_match.hpp"

#include "network/network_codec.hpp"

ServerMatch::ServerMatch(
    std::uint32_t matchId,
    int boardSize
) : board_(boardSize), gameLogic_(board_), matchId_(matchId) {}

const Board& ServerMatch::getBoard() const {
    return board_;
}

std::uint32_t& ServerMatch::expectedSequence(MatchPlayer player) {
    if (player == MatchPlayer::Black) {
        return expectedBlackSequence_;
    }

    return expectedWhiteSequence_;
}

std::vector<OutgoingMessage> ServerMatch::handleMessage(MatchPlayer sender, const WireMessage& message) {
    std::vector<OutgoingMessage> responses;

    if (message.type != network::ProtocolMessageType::MoveRequest) return responses;

    network::MoveRequest request;
    if (!NetworkCodec::decode(message, request)) return responses;
    if (request.matchId != matchId_) return responses;

    std::uint32_t& expected = expectedSequence(sender);

    if (request.sequenceNumber != expected) {
        responses.push_back(makeRejected(
            sender,
            request,
            network::MoveRejectReason::InvalidSequence
        ));
        return responses;
    }

    // A well-formed request consumes its sequence number even if
    // the requested move is rejected by the match rules.
    ++expected;

    const MatchPlayer currentPlayer = (
        gameLogic_.getCurrentPlayer() == Stone::Black
            ? MatchPlayer::Black
            : MatchPlayer::White
        );

    if (sender != currentPlayer) {
        responses.push_back(makeRejected(
            sender,
            request,
            network::MoveRejectReason::NotYourTurn
        ));
        return responses;
    }

    const MoveResult result =
        gameLogic_.placeStone(request.x, request.y);

    if (result.status != MoveStatus::Accepted) {
        responses.push_back(makeRejected(
            sender,
            request,
            convertReason(result.status)
        ));
        return responses;
    }

    ++stateVersion_;

    const network::BoardSnapshot snapshot = makeBoardSnapshot();
    WireMessage snapshotMessage;

    if (!NetworkCodec::encode(snapshot, snapshotMessage)) return responses;

    responses.push_back({
        MatchPlayer::Black,
        snapshotMessage
    });

    responses.push_back({
        MatchPlayer::White,
        snapshotMessage
    });

    return responses;
}

OutgoingMessage ServerMatch::makeRejected(
    MatchPlayer recipient,
    const network::MoveRequest& request,
    network::MoveRejectReason reason
) const {
    network::MoveRejected rejected;
    rejected.matchId = request.matchId;
    rejected.sequenceNumber = request.sequenceNumber;
    rejected.reason = reason;

    WireMessage message;
    NetworkCodec::encode(rejected, message);

    return OutgoingMessage{recipient, message};
}

network::MoveRejectReason ServerMatch::convertReason(
    MoveStatus status
) const {
    switch (status) {
    case MoveStatus::Occupied:
        return network::MoveRejectReason::Occupied;

    case MoveStatus::Suicide:
        return network::MoveRejectReason::Suicide;

    case MoveStatus::Ko:
        return network::MoveRejectReason::Ko;

    case MoveStatus::OutOfBounds:
        return network::MoveRejectReason::OutOfBounds;

    default:
        return network::MoveRejectReason::InvalidSequence;
    }
}

network::BoardSnapshot ServerMatch::makeBoardSnapshot() const {
    network::BoardSnapshot snapshot;
    snapshot.matchId = matchId_;
    snapshot.stateVersion = stateVersion_;
    snapshot.boardSize = static_cast<std::uint8_t>(
        board_.getSize()
    );
    snapshot.currentPlayer = static_cast<std::uint8_t>(
        gameLogic_.getCurrentPlayer()
    );

    const std::vector<Stone> boardState =
        board_.getBoardState();

    snapshot.stones.reserve(boardState.size());

    for (Stone stone : boardState) {
        snapshot.stones.push_back(static_cast<std::uint8_t>(stone));
    }

    return snapshot;
}
