#include "button.hpp"

#include <utility>

Button::Button(const std::string& label, std::function<void()> action)
    :   x_(0),
        y_(0),
        width_(0),
        height_(0),
        label_(label),
        hovered_(false),
        action_(std::move(action)) {}

void Button::setBounds(int x, int y, int width, int height) {
    if (x < 0 ||  y < 0 ||
        width <= 0 || height <= 0
        ) return;
    
    x_ = x;
    y_ = y;
    width_ = width;
    height_ = height;
}

void Button::setHovered(bool hovered) {
    hovered_ = hovered;
}

void Button::activate() {
    if (action_) {
        action_();
    }
}

bool Button::contains(int mouseX, int mouseY) const {
    return  mouseX >= x_ && 
            mouseX < x_ + width_ &&
            mouseY >= y_ && 
            mouseY < y_ + height_;
}

void Button::draw(IGraphicsRenderer& renderer) const {
    if (hovered_) renderer.setColor(100, 150, 220, 255); 
    else renderer.setColor(70, 110, 180, 255);

    renderer.drawFilledRect(x_, y_, width_, height_);
    renderer.drawText(
        label_, 
        x_ + width_ / 2, 
        y_ + height_ / 2,
        255,
        255,
        255,
        255
    );
}

std::string Button::getLabel() const {
    return label_;
}