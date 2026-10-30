#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include "game_logic.hpp"
#include "board_geometry.hpp"

class InputController {
public:
    InputController(
        GameLogic& game_logic,
        int window_width,
        int window_height,
        int board_size
    );

    void setWindowSize(int window_width, int window_height);

    void handleMouseClick(int mouse_x, int mouse_y);

private:
    GameLogic& game_logic_;
    BoardGeometry geometry_;
};

#endif