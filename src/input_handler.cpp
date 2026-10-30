#include "../headers/input_controller.hpp"

#include <iostream>

InputController::InputController(
    GameLogic& game_logic,
    int window_width,
    int window_height,
    int board_size
)
    : game_logic_(game_logic),
      geometry_(board_size, window_width, window_height) {
}

void InputController::setWindowSize(
    int window_width,
    int window_height
) {
    geometry_.resize(window_width, window_height);
}

void InputController::handleMouseClick(int mouse_x, int mouse_y) {
    int grid_x = 0;
    int grid_y = 0;

    if (!geometry_.screenToGrid(
            mouse_x,
            mouse_y,
            grid_x,
            grid_y)) {
        return;
    }

    const Stone currentPlayer = game_logic_.getCurrentPlayer();

    if (!game_logic_.placeStone(grid_x, grid_y, currentPlayer)) {
        std::cout << "Invalid move!\n";
    }
}
