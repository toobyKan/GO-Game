#include "headers/sdl_graphics_renderer.hpp"
#include "headers/board.hpp"
#include "headers/game_logic.hpp"

int main() {
    const int board_size = 19;
    const int window_size = 800;

    Board board(board_size);
    GameLogic gameLogic(board);
    SDLRenderer renderer(board, window_size);

    if (!renderer.initialize()) {
        return -1;  // Exit if initialization fails
    }

    board.attachObserver(&renderer);

    bool running = true;

    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x * board_size / window_size;
                int y = event.button.y * board_size / window_size;

                static Stone current_stone = Stone::Black;
                if (gameLogic.placeStone(x, y, current_stone)) {
                    current_stone = (current_stone == Stone::Black) ? Stone::White : Stone::Black;  // Switch stones
                }
            }
        }
    }

    return 0;
}