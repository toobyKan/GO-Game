#ifndef GRAPHICS_RENDERER_H
#define GRAPHICS_RENDERER_H

#include "board.hpp"

class IGraphicsRenderer {
public:
    virtual ~IGraphicsRenderer() {}

    virtual bool initialize() = 0;
    virtual void render() = 0;
    virtual void clearScreen() = 0;
    virtual void presentScreen() = 0;
};

#endif
