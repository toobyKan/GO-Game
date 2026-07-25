#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include "game_logic.hpp"

class InputController {
public:
    InputController(GameLogic& game_logic, int window_size, int board_size);
    
    void handleMouseClick(int mouse_x, int mouse_y);

private:
    GameLogic& game_logic_;
    int window_size_;
    int board_size_;
};

#endif