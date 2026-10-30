#include "headers/board.hpp"
#include "headers/game_logic.hpp"

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

void test_empty_board() {
    Board board(3);
    expect(board.getSize() == 3, "board reports its size");
    expect(board.getStoneAt(1, 1) == Stone::None,
           "new board contains no stones");
}

void test_place_and_turn_order() {
    Board board(3);
    GameLogic game(board);

    expect(game.getCurrentPlayer() == Stone::Black,
           "black starts the game");
    expect(game.placeStone(1, 1, Stone::Black),
           "black can place on an empty point");
    expect(board.getStoneAt(1, 1) == Stone::Black,
           "placed stone is stored on the board");
    expect(game.getCurrentPlayer() == Stone::White,
           "turn changes after a successful move");
    expect(!game.placeStone(1, 1, Stone::White),
           "occupied points cannot be played");
}

void test_suicide_is_rejected() {
    Board board(3);
    board.setStoneAt(1, 0, Stone::White);
    board.setStoneAt(0, 1, Stone::White);
    board.setStoneAt(2, 1, Stone::White);
    board.setStoneAt(1, 2, Stone::White);

    GameLogic game(board);
    expect(!game.placeStone(1, 1, Stone::Black),
           "a move with no liberties is rejected");
    expect(board.getStoneAt(1, 1) == Stone::None,
           "a rejected suicide does not change the board");
}

void test_invalid_moves_are_rejected() {
    Board board(3);
    GameLogic game(board);
    const std::vector<Stone> initialState = board.getBoardState();

    expect(!game.placeStone(-1, 0, Stone::Black),
           "negative coordinates are rejected");
    expect(!game.placeStone(3, 0, Stone::Black),
           "coordinates past the board are rejected");
    expect(!game.placeStone(0, 0, Stone::White),
           "the wrong player's stone is rejected");
    expect(!game.placeStone(0, 0, Stone::None),
           "an empty stone is rejected");
    expect(board.getBoardState() == initialState,
           "invalid moves do not change the board");
}

void test_capture_is_allowed() {
    Board board(3);
    board.setStoneAt(1, 1, Stone::White);
    board.setStoneAt(0, 1, Stone::Black);
    board.setStoneAt(1, 0, Stone::Black);
    board.setStoneAt(2, 1, Stone::Black);

    GameLogic game(board);
    expect(game.placeStone(1, 2, Stone::Black),
           "a move that captures an opponent group is legal");
    expect(board.getStoneAt(1, 1) == Stone::None,
           "captured stones are removed");
    expect(board.getStoneAt(1, 2) == Stone::Black,
           "the capturing stone remains on the board");
}

void test_ko_is_rejected() {
    Board board(5);

    // White at (1, 1) is captured by black at (1, 2).
    board.setStoneAt(1, 1, Stone::White);
    board.setStoneAt(0, 1, Stone::Black);
    board.setStoneAt(1, 0, Stone::Black);
    board.setStoneAt(2, 1, Stone::Black);
    board.setStoneAt(0, 2, Stone::White);
    board.setStoneAt(2, 2, Stone::White);
    board.setStoneAt(1, 3, Stone::White);

    GameLogic game(board);
    expect(game.placeStone(1, 2, Stone::Black),
           "the first move in a Ko position is legal");

    const std::vector<Stone> stateAfterCapture = board.getBoardState();
    expect(!game.placeStone(1, 1, Stone::White),
           "the immediate Ko recapture is rejected");
    expect(board.getBoardState() == stateAfterCapture,
           "a rejected Ko recapture restores the entire board");
}

} // namespace

int main() {
    test_empty_board();
    test_place_and_turn_order();
    test_suicide_is_rejected();
    test_invalid_moves_are_rejected();
    test_capture_is_allowed();
    test_ko_is_rejected();

    if (failures != 0) {
        std::cerr << failures << " test(s) failed\n";
        return 1;
    }

    std::cout << "All tests passed\n";
    return 0;
}
