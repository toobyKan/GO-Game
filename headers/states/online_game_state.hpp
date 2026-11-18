#ifndef ONLINE_GAME_STATE_H
#define ONLINE_GAME_STATE_H

#include "app_state.hpp"
#include "../online_game_session.hpp"

class StateManager;

class OnlineGameState : public AppState {
public:
    OnlineGameState(
        StateManager& state_manager,
        int board_size, 
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

    OnlineGameSession session_;

    int windowWidth_;
    int windowHeight_;
    int boardSize_;

};

#endif