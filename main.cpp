#include "sdl_graphics_renderer.hpp"
#include "board.hpp"
#include "board_view.hpp"
#include "game_logic.hpp"
#include "input_controller.hpp"
#include "states/state_manager.hpp"
#include "states/main_menu_state.hpp"

#include <iostream>
#include <chrono>

using Clock = std::chrono::steady_clock;


int main() {
    const int board_size = 19;
    const int window_width = 800;
    const int window_height = 800;

    SDLRenderer renderer(window_width);
    
    if (!renderer.initialize()) {
        return -1;
    }

    if (!renderer.loadFont(
            "assets/DejaVuSans.ttf",
            28)) {
        return -1;
    }

    StateManager stateManager;

    stateManager.replaceState(
        std::unique_ptr<AppState>(
            new MainMenuState(
                stateManager,
                window_width,
                window_height,
                board_size
            )
        )
    );

    auto previousTime = Clock::now();

    while (stateManager.isRunning()) {
        auto currentTime = Clock::now();

        std::chrono::duration<double> elapsed = currentTime - previousTime;
        previousTime = currentTime;

        double deltaTime = elapsed.count();

        if (deltaTime > 0.1) deltaTime = 0.1;

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            stateManager.handleEvent(event);
        }

        stateManager.update(deltaTime);

        renderer.clearScreen();
        stateManager.render(renderer);
        renderer.presentScreen();
    }

    return 0;
}