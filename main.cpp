#include "headers/sdl_graphics_renderer.hpp"
#include "headers/board.hpp"
#include "headers/game_logic.hpp"

#include <iostream>

int main() {
    const int board_size = 19;
    const int window_size = 800;

    Board board(board_size);
    GameLogic gameLogic(board);
    SDLRenderer renderer(board, window_size);

    if (!renderer.initialize()) {
        return -1;  // Exit if initialization fails
    }

    gameLogic.attachObserver(&renderer);

    bool running = true;

    SDL_Event event;
    renderer.render();

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x * board_size / window_size;
                int y = event.button.y * board_size / window_size;

                Stone currentPlayer = gameLogic.getCurrentPlayer();
                if (!gameLogic.placeStone(x, y, currentPlayer)) {
                    std::cout << "Invalid move!" << std::endl;
                }
            }
        }
    }

    return 0;
}