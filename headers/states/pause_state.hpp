#ifndef PAUSE_STATE
#define PAUSE_STATE

#include "app_state.hpp"
#include "../button.hpp"
#include <vector>
#include <memory>


class StateManager;


class PauseState : public AppState {
public:
    PauseState(
        StateManager& stateManager,
        int windowWidth,
        int windowHeight,
        int boardSize
    );

    void onEnter() override;
    void onWindowResized(int width, int height) override;
    void handleEvent(const SDL_Event& event) override;
    void update(double deltaTime) override;
    void render(IGraphicsRenderer& renderer) override;

private:
    void updateLayout();
    void registerButtons();

    StateManager& stateManager_;
    std::vector<std::unique_ptr<Button>> buttons_;

    int windowWidth_;
    int windowHeight_;
    int boardSize_;
};

#endif