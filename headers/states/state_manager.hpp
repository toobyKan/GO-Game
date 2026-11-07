#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include "app_state.hpp"

#include <memory>
#include <vector>

class StateManager {
public:
    void replaceState(std::unique_ptr<AppState> state);
    void resetState(std::unique_ptr<AppState> state);
    void pushState(std::unique_ptr<AppState> state);
    void popState();

    void handleEvent(const SDL_Event& event);
    void update(double deltaTime);
    void render(IGraphicsRenderer& renderer);

    bool isRunning() const;
    void stop();

private:
    enum class PendingTransition {
        None,
        Replace,
        Push,
        Pop,
        Reset
    };

    void applyPendingTransition();

    std::vector<std::unique_ptr<AppState>> stateStack_;
    std::unique_ptr<AppState> pendingState_;
    PendingTransition pendingTransition_ = PendingTransition::None;
    bool running_ = true;
};

#endif
