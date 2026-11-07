#include "sdl_graphics_renderer.hpp"
#include <iostream>

SDLRenderer::SDLRenderer(int window_size)
    : window_(nullptr), renderer_(nullptr), font_(nullptr), window_size_(window_size) {
}

SDLRenderer::~SDLRenderer() {
    if (font_) {
        TTF_CloseFont(font_);
    }

    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);

    TTF_Quit();
    SDL_Quit();
}

bool SDLRenderer::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << '\n';
        
        return false;
    }

    if (TTF_Init() < 0) {
        std::cerr << "TTF_Init failed: "
                << TTF_GetError() << '\n';

        SDL_Quit();
        return false;
    }

    window_ = SDL_CreateWindow(
        "Go", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        window_size_, 
        window_size_, 
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    if (!window_) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << '\n';
        SDL_Quit();
        
        return false;
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    
    if (!renderer_) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window_);
        window_ = nullptr;
        SDL_Quit();

        return false;
    }

    return true;
}


void SDLRenderer::render() {}

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


void SDLRenderer::drawFilledRect(int x, int y, int width, int height) {
    SDL_Rect rectangle {x, y, width, height};

    SDL_RenderFillRect(renderer_, &rectangle);
}

void SDLRenderer::drawText(
    const std::string& text,
    int centerX,
    int centerY,
    uint8_t r,
    uint8_t g,
    uint8_t b,
    uint8_t a
) {
    if (!font_ || text.empty()) {
        return;
    }

    SDL_Color color {r, g, b, a};

    SDL_Surface* surface =
        TTF_RenderUTF8_Blended(
            font_,
            text.c_str(),
            color
        );

    if (!surface) {
        std::cerr << "TTF_RenderUTF8_Blended failed: "
                  << TTF_GetError() << '\n';
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);

    if (!texture) {
        std::cerr << "SDL_CreateTextureFromSurface failed: "
                  << SDL_GetError() << '\n';

        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect destination {
        centerX - surface->w / 2,
        centerY - surface->h / 2,
        surface->w,
        surface->h
    };

    SDL_SetTextureBlendMode(
        texture,
        SDL_BLENDMODE_BLEND
    );

    SDL_RenderCopy(
        renderer_,
        texture,
        nullptr,
        &destination
    );

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

bool SDLRenderer::loadFont(
    const std::string& font_path,
    int font_size
) {
    if (font_) {
        TTF_CloseFont(font_);
        font_ = nullptr;
    }

    font_ = TTF_OpenFont(
        font_path.c_str(),
        font_size
    );

    if (!font_) {
        std::cerr << "TTF_OpenFont failed: "
                  << TTF_GetError() << '\n';
        return false;
    }

    return true;
}