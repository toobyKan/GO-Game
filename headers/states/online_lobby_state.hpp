#ifndef ONLINE_LOBBY_STATE_H
#define ONLINE_LOBBY_STATE_H

#include "app_state.hpp"


class StateManager;

class OnlineLobbyState : public AppState {
public:
    OnlineLobbyState(
        StateManager& state_manager,
        int window_width, 
        int window_height
    );

    void onEnter() override;
    void onExit() override;

    void onPause() override;
    void onResume() override;

    void onWindowResized(int width, int height) override;


    void handleEvent(const SDL_Event& event) override;
    void update(double deltaTime) override;
    void render(IGraphicsRenderer& renderer) override;

private:
    StateManager& stateManager_;


    int windowWidth_;
    int windowHeight_;

};

#endif