#include "headers/sdl_graphics_renderer.hpp"
#include "headers/board.hpp"
#include "headers/board_view.hpp"
#include "headers/game_logic.hpp"
#include "headers/input_controller.hpp"

#include <iostream>

int main() {
    const size_t board_size = 19;
    const int window_size = 800;

    Board board(board_size);
    GameLogic gameLogic(board);

    SDLRenderer renderer(window_size);
    BoardView boardView(board, window_size);
    InputController input(gameLogic, window_size, board_size);
    
    if (!renderer.initialize()) {
        return -1;
    }

    renderer.addEntity(&boardView);

    // Logic to update the screen after a move
    gameLogic.onTurnCompleted.push_back([&renderer]() {
        renderer.render();
    });

    bool running = true;
    SDL_Event event;
    
    // Initial draw
    renderer.render();

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                input.handleMouseClick(event.button.x, event.button.y);
            }
        }
    }

    return 0;
}