#include "../headers/sdl_graphics_renderer.hpp"
#include <iostream>

SDLRenderer::SDLRenderer(Board& board, int window_size)
    : board_(board), window_(nullptr), renderer_(nullptr), window_size_(window_size) {
    tile_size_ = window_size_ / board_.getSize();
    board_.attachObserver(this);
}

SDLRenderer::~SDLRenderer() {
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

bool SDLRenderer::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }

    window_ = SDL_CreateWindow("Go", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_size_, window_size_, SDL_WINDOW_SHOWN);
    if (!window_) {
        return false;
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    return renderer_ != nullptr;
}

void SDLRenderer::render() {
    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);  // White background
    SDL_RenderClear(renderer_);

    clearScreen();
    drawBoardGrid();
    drawStones();

    SDL_RenderPresent(renderer_);
}

void SDLRenderer::update() {
    render();
}

void SDLRenderer::clearScreen() {
    SDL_SetRenderDrawColor(renderer_, 205, 170, 125, 255);  // Light brown background
    SDL_RenderClear(renderer_);
}

void SDLRenderer::presentScreen() {
    SDL_RenderPresent(renderer_);
}

void SDLRenderer::drawBoardGrid() {
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);  // Black color for lines

    // Draw vertical lines
    for (int i = 0; i < board_.getSize(); ++i) {
        int x = i * tile_size_;
        SDL_RenderDrawLine(renderer_, x, 0, x, window_size_);
    }

    // Draw horizontal lines
    for (int i = 0; i < board_.getSize(); ++i) {
        int y = i * tile_size_;
        SDL_RenderDrawLine(renderer_, 0, y, window_size_, y);
    }
}

void SDLRenderer::drawStones() {
    for (int x = 0; x < board_.getSize(); ++x) {
        for (int y = 0; y < board_.getSize(); ++y) {
            Stone stone = board_.getStoneAt(x, y);
            if (stone != Stone::None) {
                drawStone(x, y, stone);
            }
        }
    }
}

void SDLRenderer::drawStone(int x, int y, Stone stone) {
    // Set the color for the stone
    if (stone == Stone::Black) {
        SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);  // Black
    } else {
        SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);  // White
    }

    // Calculate the position of the stone
    int stone_x = x * tile_size_ + tile_size_ / 2;
    int stone_y = y * tile_size_ + tile_size_ / 2;
    int radius = tile_size_ / 3;  // Adjust radius based on tile size

    filledCircle(stone_x, stone_y, radius);  // Draw filled circle for stone
}

void SDLRenderer::filledCircle(int cx, int cy, int radius) {
    // Draw a filled circle
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;  // Horizontal offset
            int dy = radius - h;  // Vertical offset
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer_, cx + dx, cy + dy);
            }
        }
    }
}