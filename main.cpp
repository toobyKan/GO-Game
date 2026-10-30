#include "headers/sdl_graphics_renderer.hpp"
#include "headers/board.hpp"
#include "headers/board_view.hpp"
#include "headers/game_logic.hpp"
#include "headers/input_controller.hpp"

#include <iostream>

int main() {
    const size_t board_size = 19;
    const int window_width = 800;
    const int window_height = 800;

    Board board(board_size);
    GameLogic gameLogic(board);

    SDLRenderer renderer(window_width);
    BoardView boardView(board, window_width, window_height);
    InputController input(gameLogic, window_width, window_height, board_size);
    
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
        if (SDL_WaitEvent(&event) == 0) {
            std::cerr << "SDL_WaitEvent failed: " << SDL_GetError() << '\n';
            break;
        }
           
        switch (event.type) {
        case SDL_QUIT:
            running = false;
            break;

        case SDL_MOUSEBUTTONDOWN:
            input.handleMouseClick(event.button.x, event.button.y);
            break;
 
        case SDL_WINDOWEVENT:
            if (event.window.event ==
                SDL_WINDOWEVENT_SIZE_CHANGED) {
                const int width = event.window.data1;
                const int height = event.window.data2;

                boardView.setWindowSize(width, height);
                input.setWindowSize(width, height);

                renderer.render();
            }
            break;

        default:
            break;
        }
    }

    return 0;
}