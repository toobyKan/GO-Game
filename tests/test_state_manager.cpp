#include "states/state_manager.hpp"
#include "graphics_renderer.hpp"

#include <iostream>
#include <memory>
#include <string>

namespace {

int failures = 0;

void expect(bool condition, const std::string& description) {
    if (!condition) {
        ++failures;
        std::cerr << "FAIL: " << description << '\n';
    }
}

struct StateCounters {
    int entered = 0;
    int exited = 0;
    int paused = 0;
    int resumed = 0;
    int events = 0;
    int updates = 0;
    int renders = 0;
};

class NullRenderer : public IGraphicsRenderer {
public:
    bool initialize() override { return true; }
    void render() override {}
    void clearScreen() override {}
    void presentScreen() override {}

    void setColor(
        uint8_t,
        uint8_t,
        uint8_t,
        uint8_t = 255
    ) override {}

    void drawLine(int, int, int, int) override {}
    void drawFilledCircle(int, int, int) override {}
    void drawFilledRect(int, int, int, int) override {}

    void drawText(
        const std::string&,
        int,
        int,
        uint8_t,
        uint8_t,
        uint8_t,
        uint8_t = 255
    ) override {}
};

class TestState : public AppState {
public:
    explicit TestState(StateCounters& counters)
        : counters_(counters) {
    }

    void onEnter() override {
        ++counters_.entered;
    }

    void onExit() override {
        ++counters_.exited;
    }

    void onPause() override {
        ++counters_.paused;
    }

    void onResume() override {
        ++counters_.resumed;
    }

    void handleEvent(const SDL_Event&) override {
        ++counters_.events;
    }

    void update(double) override {
        ++counters_.updates;
    }

    void render(IGraphicsRenderer&) override {
        ++counters_.renders;
    }

private:
    StateCounters& counters_;
};

std::unique_ptr<AppState> makeState(StateCounters& counters) {
    return std::unique_ptr<AppState>(new TestState(counters));
}

void test_replace_enters_state() {
    StateManager manager;
    StateCounters counters;

    manager.replaceState(makeState(counters));

    expect(counters.entered == 0,
           "replace is deferred until update");

    manager.update(0.016);

    expect(counters.entered == 1,
           "replacement state receives onEnter");
    expect(counters.updates == 1,
           "active state receives update");
}

void test_push_and_pop_lifecycle() {
    StateManager manager;
    StateCounters base;
    StateCounters overlay;

    manager.replaceState(makeState(base));
    manager.update(0.016);

    manager.pushState(makeState(overlay));
    manager.update(0.016);

    expect(base.paused == 1,
           "pushing a state pauses the previous state");
    expect(overlay.entered == 1,
           "pushed state receives onEnter");

    manager.popState();
    manager.update(0.016);

    expect(overlay.exited == 1,
           "popped state receives onExit");
    expect(base.resumed == 1,
           "underlying state receives onResume");
}

void test_only_top_state_receives_events_and_updates() {
    StateManager manager;
    StateCounters base;
    StateCounters overlay;

    manager.replaceState(makeState(base));
    manager.update(0.016);
    manager.pushState(makeState(overlay));
    manager.update(0.016);

    SDL_Event event{};
    event.type = SDL_MOUSEBUTTONDOWN;
    manager.handleEvent(event);
    manager.update(0.016);

    expect(base.events == 0,
           "underlying state does not receive input");
    expect(overlay.events == 1,
           "top state receives input");
    expect(base.updates == 1,
           "underlying state stops updating while paused");
    expect(overlay.updates == 2,
           "top state receives update");
}

void test_rendering_includes_underlying_states() {
    StateManager manager;
    StateCounters base;
    StateCounters overlay;
    NullRenderer renderer;

    manager.replaceState(makeState(base));
    manager.update(0.016);
    manager.pushState(makeState(overlay));
    manager.update(0.016);

    manager.render(renderer);

    expect(base.renders == 1,
           "underlying state is rendered for overlays");
    expect(overlay.renders == 1,
           "top state is rendered");
}

void test_replace_replaces_only_top_state() {
    StateManager manager;
    StateCounters base;
    StateCounters overlay;
    StateCounters replacement;

    manager.replaceState(makeState(base));
    manager.update(0.016);
    manager.pushState(makeState(overlay));
    manager.update(0.016);

    manager.replaceState(makeState(replacement));
    manager.update(0.016);

    expect(base.exited == 0,
           "replace preserves the underlying state");
    expect(overlay.exited == 1,
           "replace exits only the top state");
    expect(replacement.entered == 1,
           "replace enters the replacement state");
}
void test_reset_clears_all_states() {
    StateManager manager;
    StateCounters base;
    StateCounters overlay;
    StateCounters replacement;

    manager.replaceState(makeState(base));
    manager.update(0.016);
    manager.pushState(makeState(overlay));
    manager.update(0.016);

    manager.resetState(makeState(replacement));
    manager.update(0.016);

    expect(base.exited == 1,
           "reset exits the bottom state");
    expect(overlay.exited == 1,
           "reset exits the overlay state");
    expect(replacement.entered == 1,
           "reset enters the new state");
}

void test_quit_stops_manager() {
    StateManager manager;
    StateCounters counters;

    manager.replaceState(makeState(counters));
    manager.update(0.016);

    SDL_Event event{};
    event.type = SDL_QUIT;
    manager.handleEvent(event);

    expect(!manager.isRunning(),
           "SDL_QUIT stops the state manager");
    expect(counters.events == 0,
           "SDL_QUIT is not forwarded to the state");
}

} // namespace

int main() {
    test_replace_enters_state();
    test_push_and_pop_lifecycle();
    test_only_top_state_receives_events_and_updates();
    test_rendering_includes_underlying_states();
    test_replace_replaces_only_top_state();
    test_reset_clears_all_states();
    test_quit_stops_manager();

    if (failures != 0) {
        std::cerr << failures << " test(s) failed\n";
        return 1;
    }

    std::cout << "All state-manager tests passed\n";
    return 0;
}




