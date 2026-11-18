#include "local_game_session.hpp"

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

void test_session_starts_with_empty_board() {
    LocalGameSession session(3);
    const Board& board = session.getBoard();

    expect(board.getSize() == 3,
           "session creates a board with the requested size");
    expect(board.getStoneAt(1, 1) == Stone::None,
           "session board starts empty");
    expect(session.getCurrentPlayer() == Stone::Black,
           "session starts with black as the current player");
}

void test_successful_move_updates_session() {
    LocalGameSession session(3);

    expect(session.submitMove(1, 1),
           "session accepts a legal move");
    expect(session.getBoard().getStoneAt(1, 1) == Stone::Black,
           "accepted move updates the session board");
    expect(session.getCurrentPlayer() == Stone::White,
           "accepted move changes the current player");
}

void test_rejected_move_preserves_session_state() {
    LocalGameSession session(3);

    expect(session.submitMove(1, 1),
           "initial move is accepted before testing rejection");

    const std::vector<Stone> stateAfterFirstMove =
        session.getBoard().getBoardState();
    const Stone playerAfterFirstMove = session.getCurrentPlayer();

    expect(!session.submitMove(1, 1),
           "session rejects a move on an occupied point");
    expect(session.getBoard().getBoardState() == stateAfterFirstMove,
           "rejected move does not change the board");
    expect(session.getCurrentPlayer() == playerAfterFirstMove,
           "rejected move does not change the current player");
}

void test_out_of_bounds_move_is_rejected() {
    LocalGameSession session(3);
    const std::vector<Stone> initialState =
        session.getBoard().getBoardState();

    expect(!session.submitMove(-1, 0),
           "session rejects negative coordinates");
    expect(!session.submitMove(3, 0),
           "session rejects coordinates past the board");
    expect(session.getBoard().getBoardState() == initialState,
           "out-of-bounds moves do not change the board");
    expect(session.getCurrentPlayer() == Stone::Black,
           "out-of-bounds moves do not change the current player");
}

} // namespace

int main() {
    test_session_starts_with_empty_board();
    test_successful_move_updates_session();
    test_rejected_move_preserves_session_state();
    test_out_of_bounds_move_is_rejected();


    if (failures != 0) {
        std::cerr << failures << " test(s) failed\n";
        return 1;
    }

    std::cout << "All local-session tests passed\n";
    return 0;
}