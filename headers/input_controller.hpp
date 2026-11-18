#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include "board_geometry.hpp"

#include <functional>

class InputController {
public:
    InputController(
        int window_width,
        int window_height,
        int board_size,
        std::function<void(int, int)> onMoveRequested
    );

    void setWindowSize(int window_width, int window_height);

    void handleMouseClick(int mouse_x, int mouse_y);

private:
    BoardGeometry geometry_;
    std::function<void(int, int)> onMoveRequested_;
};

#endif