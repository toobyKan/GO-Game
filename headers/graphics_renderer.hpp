#ifndef GRAPHICS_RENDERER_H
#define GRAPHICS_RENDERER_H

#include <cstdint>

class IGraphicsRenderer {
public:
    virtual ~IGraphicsRenderer() {}

    virtual bool initialize() = 0;
    virtual void render() = 0;
    virtual void clearScreen() = 0;
    virtual void presentScreen() = 0;

    // Core drawing primitives
    virtual void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) = 0;
    virtual void drawLine(int x1, int y1, int x2, int y2) = 0;
    virtual void drawFilledCircle(int cx, int cy, int radius) = 0;
};

#endif