#include "../headers/input_controller.hpp"
#include <iostream>

InputController::InputController(GameLogic& game_logic, int window_size, int board_size)
    : game_logic_(game_logic), window_size_(window_size), board_size_(board_size) {}

void InputController::handleMouseClick(int mouse_x, int mouse_y) {
    int grid_x = (mouse_x * board_size_) / window_size_;
    int grid_y = (mouse_y * board_size_) / window_size_;

    Stone currentPlayer = game_logic_.getCurrentPlayer();
    if (!game_logic_.placeStone(grid_x, grid_y, currentPlayer)) {
        std::cout << "Invalid move!" << std::endl;
    }
}