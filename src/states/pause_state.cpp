#include "states/pause_state.hpp"

#include "states/state_manager.hpp"
#include "states/main_menu_state.hpp"
#include "graphics_renderer.hpp"

#include <memory>
#include <algorithm>


PauseState::PauseState(
    StateManager& stateManager,
    int windowWidth,
    int windowHeight,
    int boardSize
)
    : stateManager_(stateManager),
      windowWidth_(windowWidth),
      windowHeight_(windowHeight),
      boardSize_(boardSize) {
    registerButtons();
    updateLayout();
}

void PauseState::registerButtons() {
    buttons_.push_back(
        std::unique_ptr<Button>(
            new Button(
                "Resume",
                [this]() {
                    stateManager_.popState();
                }
            )
        )
    );

    buttons_.push_back(
        std::unique_ptr<Button>(
            new Button(
                "Main Menu",
                [this]() {
                    stateManager_.resetState(
                        std::unique_ptr<AppState>(
                            new MainMenuState(
                                stateManager_,
                                windowWidth_,
                                windowHeight_,
                                boardSize_
                            )
                        )
                    );
                }
            )
        )
    );

    buttons_.push_back(
        std::unique_ptr<Button>(
            new Button(
                "Exit",
                [this]() {
                    stateManager_.stop();
                }
            )
        )
    );
}

void PauseState::updateLayout() {
    const int buttonWidth = 260;
    const int buttonHeight = 60;
    const int spacing = 20;
    int buttonCount = 0;

    const int totalHeight =
        buttonHeight * 3 + spacing * 2;

    const int startX = std::max(0, (windowWidth_ - buttonWidth) / 2);

    const int startY =std::max(0, (windowHeight_ - totalHeight) / 2);

    for (const auto& button : buttons_) {
        button->setBounds(
            startX,
            startY + (buttonHeight + spacing) * buttonCount,
            buttonWidth,
            buttonHeight
        );
        buttonCount++;
    }
}

void PauseState::onEnter() {
    updateLayout();
}

void PauseState::onWindowResized(int width, int height) {
    windowWidth_ = width;
    windowHeight_ = height;

    updateLayout();
}

void PauseState::handleEvent(
    const SDL_Event& event
) {
    switch (event.type) {
    case SDL_MOUSEMOTION:
        for (const auto& button : buttons_) {
            button->setHovered(
                button->contains(
                    event.motion.x,
                    event.motion.y
                )
            );
        }
        break;

    case SDL_MOUSEBUTTONDOWN:
        for (const auto& button : buttons_) {
            if (button->contains(event.button.x, event.button.y)) {
                button->activate();
                break;
            }
        }
        break;

    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE){
            for (const auto& button : buttons_) {
                if (button->getLabel() == "Resume"){
                    button->activate();
                    break;
                }
            }
        }
        break;

    case SDL_WINDOWEVENT:
        if (event.window.event ==
            SDL_WINDOWEVENT_SIZE_CHANGED) {
            windowWidth_ = event.window.data1;
            windowHeight_ = event.window.data2;
            updateLayout();
        }
        break;

    default:
        break;
    }
}

void PauseState::update(double deltaTime) {
    (void)deltaTime;
}

void PauseState::render(
    IGraphicsRenderer& renderer
) {
    for (const auto& button : buttons_) {
        button->draw(renderer);
    }
}