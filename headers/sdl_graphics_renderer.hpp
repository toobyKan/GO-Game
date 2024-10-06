#ifndef SDL_RENDERER_H
#define SDL_RENDERER_H

#include <SDL2/SDL.h>
#include "graphics_renderer.hpp"
#include "board.hpp"
#include "observer.hpp"

class SDLRenderer : public IGraphicsRenderer, public Observer{
public:
    SDLRenderer(Board& board, int window_size);
    ~SDLRenderer();

    // Override methods from GraphicalInterface
    bool initialize() override;
    void render() override;
    void clearScreen() override;
    void presentScreen() override;

    void update() override;

private:
    void drawBoardGrid();
    void drawStones();
    void drawStone(int x, int y, Stone stone);
    void filledCircle(int cx, int cy, int radius);

    Board& board_;
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    int window_size_;
    int tile_size_;
};

#endif  // SDL_RENDERER_H
