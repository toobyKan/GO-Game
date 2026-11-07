#ifndef BUTTON_H
#define BUTTON_H

#include "drawable_entity.hpp"

#include <string>
#include <functional>

class Button : public Drawable_Entity {
public:
    Button(const std::string& label, std::function<void()> action);
    void setBounds(int x, int y, int width, int height);
    void setHovered(bool hovered);
    void activate();
    bool contains(int mouseX, int mouseY) const;

    void draw(IGraphicsRenderer& renderer) const override;

    std::string getLabel() const;

private:
    int x_;
    int y_;
    int width_;
    int height_;

    std::string label_;
    bool hovered_;
    std::function<void()> action_;
};

#endif
