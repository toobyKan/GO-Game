#ifndef LOCAL_GAME_STATE_H
#define LOCAL_GAME_STATE_H

#include "app_state.hpp"
#include "../board.hpp"
#include "../game_logic.hpp"
#include "../board_view.hpp"
#include "../input_controller.hpp"

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

private:
    StateManager& stateManager_;

    Board board_;
    GameLogic gameLogic_;
    BoardView boardView_;
    InputController input_;

    int windowWidth_;
    int windowHeight_;
    int boardSize_;
};

#endif