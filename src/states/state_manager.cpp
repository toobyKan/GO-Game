#include "states/state_manager.hpp"


void StateManager::replaceState(std::unique_ptr<AppState> state) {
    pendingState_  = std::move(state);
    pendingTransition_ = PendingTransition::Replace;
}

void StateManager::resetState(std::unique_ptr<AppState> state) {
    pendingState_  = std::move(state);
    pendingTransition_ = PendingTransition::Reset;
}

void StateManager::pushState(std::unique_ptr<AppState> state) {
    pendingState_  = std::move(state);
    pendingTransition_ = PendingTransition::Push;
}

void StateManager::popState() {
    pendingTransition_ = PendingTransition::Pop;
}

void StateManager::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_QUIT) {
        stop();
        return;
    }
    
    if (event.type == SDL_WINDOWEVENT &&
        event.window.event ==
            SDL_WINDOWEVENT_SIZE_CHANGED) {
        const int width = event.window.data1;
        const int height = event.window.data2;

        for (auto& state : stateStack_) {
            state->onWindowResized(width, height);
        }
    }

    if (!stateStack_.empty()) {
        stateStack_.back()->handleEvent(event);
    }
}

void StateManager::update(double deltaTime){
    applyPendingTransition();
    
    if (!stateStack_.empty()) {
        stateStack_.back()->update(deltaTime);
    }
}

void StateManager::applyPendingTransition() {
    if (pendingTransition_ == PendingTransition::None) return;

    switch (pendingTransition_)
    {
    case PendingTransition::Replace:
        if (!stateStack_.empty()) {
            stateStack_.back()->onExit();
            stateStack_.pop_back();
        }
        if (pendingState_) {
            stateStack_.push_back(std::move(pendingState_));
            stateStack_.back()->onEnter();
        }
        break;

    case PendingTransition::Reset:
        while (!stateStack_.empty()) {
            stateStack_.back()->onExit();
            stateStack_.pop_back();
        }
        if (pendingState_) {
            stateStack_.push_back(std::move(pendingState_));
            stateStack_.back()->onEnter();
        }
        break;
    
    case PendingTransition::Push:
        if (!pendingState_) break;
        
        if (!stateStack_.empty()) {
            stateStack_.back()->onPause();
        }

        stateStack_.push_back(std::move(pendingState_));
        stateStack_.back()->onEnter();
        break;
    
    case PendingTransition::Pop:
        if (!stateStack_.empty()) {
            stateStack_.back()->onExit();
            stateStack_.pop_back();
        }

        if (!stateStack_.empty()) {
            stateStack_.back()->onResume();
        }
        else {
            stop();
        }
        break;
    
    case PendingTransition::None:
        break;

    default:
        break;
    }

    pendingTransition_ = PendingTransition::None;
}

void StateManager::render(IGraphicsRenderer& renderer){
    for (const auto& state : stateStack_) {
        state->render(renderer);
    }
}

bool StateManager::isRunning() const {
    return running_;
}

void StateManager::stop() {
    running_ = false;
}

