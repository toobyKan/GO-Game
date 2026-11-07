#include "states/local_game_state.hpp"

#include "states/state_manager.hpp"
#include "states/pause_state.hpp"

#include <memory>

LocalGameState::LocalGameState(
    StateManager& state_manager,
    int board_size, 
    int window_width, 
    int window_height
) :
stateManager_(state_manager),  
board_(board_size), 
gameLogic_(board_), 
boardView_(board_, window_width, window_height),
input_(gameLogic_, window_width, window_height, board_size),
windowWidth_(window_width),
windowHeight_(window_height),
boardSize_(board_size) {}

void LocalGameState::onEnter(){
    boardView_.setWindowSize(
        windowWidth_,
        windowHeight_
    );
    
    input_.setWindowSize(
        windowWidth_,
        windowHeight_
    );
}

void LocalGameState::onExit() {
    gameLogic_.onTurnCompleted.clear();
}

void LocalGameState::onPause() {}

void LocalGameState::onResume() {
    boardView_.setWindowSize(
        windowWidth_,
        windowHeight_
    );
    
    input_.setWindowSize(
        windowWidth_,
        windowHeight_
    );
}

void LocalGameState::onWindowResized(int width, int height) {
    windowWidth_ = width;
    windowHeight_ = height;

    boardView_.setWindowSize(
        windowWidth_,
        windowHeight_
    );
    
    input_.setWindowSize(
        windowWidth_,
        windowHeight_
    );
}

void LocalGameState::handleEvent(const SDL_Event& event) {
    switch (event.type) {
    case SDL_MOUSEBUTTONDOWN:
        input_.handleMouseClick(event.button.x, event.button.y);
        break;
    
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE){
            stateManager_.pushState(
                std::unique_ptr<AppState>(
                            new PauseState(
                                stateManager_,
                                windowWidth_,
                                windowHeight_,
                                boardSize_
                            )
                        )
            );
        }
        break;
    
    case SDL_WINDOWEVENT:
        if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {

            windowWidth_ = event.window.data1;
            windowHeight_= event.window.data2;

            boardView_.setWindowSize(windowWidth_, windowHeight_);
            input_.setWindowSize(windowWidth_, windowHeight_);
        }
        break;

    default:
        break;
    }
}

void LocalGameState::update(double deltaTime) {
    (void)deltaTime;
}

void LocalGameState::render(IGraphicsRenderer& renderer) {
    boardView_.draw(renderer);
}