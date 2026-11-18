#include "input_controller.hpp"


InputController::InputController(
    int window_width,
    int window_height,
    int board_size,
    std::function<void(int, int)> onMoveRequested
)
    : geometry_(board_size, window_width, window_height),
      onMoveRequested_(onMoveRequested) {
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
    if (onMoveRequested_) {
        onMoveRequested_(grid_x, grid_y);
    }
}
