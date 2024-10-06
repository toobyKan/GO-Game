#include "headers/sdl_graphics_renderer.hpp"
#include "headers/board.hpp"
#include "headers/game_logic.hpp"

int main() {
    const int board_size = 19;  // Example size for the Go board
    const int window_size = 800;  // Example window size

    Board board(board_size);
    GameLogic gameLogic(board);
    SDLRenderer renderer(board, window_size);

    if (!renderer.initialize()) {
        return -1;  // Exit if initialization fails
    }

    // Attach renderer as observer
    board.attachObserver(&renderer);

    bool running = true;

    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x * board_size / window_size;  // Scale to board size
                int y = event.button.y * board_size / window_size;  // Scale to board size

                // Example: Alternate placing stones
                static Stone current_stone = Stone::Black;
                if (gameLogic.placeStone(x, y, current_stone)) {
                    current_stone = (current_stone == Stone::Black) ? Stone::White : Stone::Black;  // Switch stones
                }
            }
        }
        // Additional game loop logic could go here (e.g., handling turns, etc.)
    }

    return 0;
}