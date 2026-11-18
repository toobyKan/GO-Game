#ifndef LOCAL_GAME_STATE_H
#define LOCAL_GAME_STATE_H

#include "app_state.hpp"
#include "../board_view.hpp"
#include "../input_controller.hpp"
#include "../local_game_session.hpp"

#include <string>

class StateManager;

class LocalGameState : public AppState{
public:
    LocalGameState(
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

    void handleMoveRequest(int x, int y);

private:
    StateManager& stateManager_;

    LocalGameSession session_;
    BoardView boardView_;
    InputController input_;

    int windowWidth_;
    int windowHeight_;
    int boardSize_;

    std::string statusMessage_;
    double statusMessageTimer_ = 0.0;
};

#endif