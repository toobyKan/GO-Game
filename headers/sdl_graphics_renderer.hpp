#ifndef SDL_RENDERER_H
#define SDL_RENDERER_H

#include <SDL2/SDL.h>
#include <vector>
#include "graphics_renderer.hpp"
#include "drawable_entity.hpp"

class SDLRenderer : public IGraphicsRenderer {
public:
    SDLRenderer(int window_size);
    ~SDLRenderer();

    bool initialize() override;
    void render() override;
    void clearScreen() override;
    void presentScreen() override;

    // New methods to manage entities and draw primitives
    void addEntity(Drawable_Entity* entity);
    
    void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;
    void drawLine(int x1, int y1, int x2, int y2) override;
    void drawFilledCircle(int cx, int cy, int radius) override;

private:
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    int window_size_;
    std::vector<Drawable_Entity*> entities_;
};

#endif