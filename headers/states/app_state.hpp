#ifndef APP_STATE_H
#define APP_STATE_H

#include <SDL2/SDL_events.h>

class IGraphicsRenderer;

class AppState {
public:
    virtual ~AppState() = default;

    virtual void onEnter() {}
    virtual void onExit() {}

    virtual void onPause() {}
    virtual void onResume() {}

    virtual void onWindowResized(int width, int height) {}

    virtual void handleEvent(const SDL_Event& event) = 0;
    virtual void update(double deltaTime) = 0;
    virtual void render(IGraphicsRenderer& renderer) = 0;
};

#endif