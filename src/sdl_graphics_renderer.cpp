#include "../headers/sdl_graphics_renderer.hpp"
#include <iostream>

SDLRenderer::SDLRenderer(int window_size)
    : window_(nullptr), renderer_(nullptr), window_size_(window_size) {
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

void SDLRenderer::addEntity(Drawable_Entity* entity) {
    entities_.push_back(entity);
}

void SDLRenderer::render() {
    clearScreen();

    // Tell all entities to draw themselves
    for (const auto& entity : entities_) {
        entity->draw(*this);
    }

    presentScreen();
}

void SDLRenderer::clearScreen() {
    SDL_SetRenderDrawColor(renderer_, 205, 170, 125, 255);  // Light brown background
    SDL_RenderClear(renderer_);
}

void SDLRenderer::presentScreen() {
    SDL_RenderPresent(renderer_);
}

void SDLRenderer::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(renderer_, r, g, b, a);
}

void SDLRenderer::drawLine(int x1, int y1, int x2, int y2) {
    SDL_RenderDrawLine(renderer_, x1, y1, x2, y2);
}

void SDLRenderer::drawFilledCircle(int cx, int cy, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer_, cx + dx, cy + dy);
            }
        }
    }
}