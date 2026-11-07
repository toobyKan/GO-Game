#ifndef SDL_RENDERER_H
#define SDL_RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

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
    
    void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;
    void drawLine(int x1, int y1, int x2, int y2) override;
    void drawFilledCircle(int cx, int cy, int radius) override;
    void drawFilledRect(int x, int y, int width, int height) override;
    void drawText(
        const std::string& text,
        int centerX,
        int centerY,
        uint8_t r,
        uint8_t g,
        uint8_t b,
        uint8_t a = 255
    ) override;

    bool loadFont(const std::string& font_path, int font_size);

private:
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    TTF_Font* font_;

    int window_size_;
};

#endif