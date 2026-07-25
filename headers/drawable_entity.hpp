#ifndef DRAWABLE_ENTITY_H
#define DRAWABLE_ENTITY_H

#include "graphics_renderer.hpp"

class Drawable_Entity {
public:
    virtual ~Drawable_Entity() = default;
    
    // Entities now draw themselves using the provided renderer interface
    virtual void draw(IGraphicsRenderer& renderer) const = 0;
};

#endif