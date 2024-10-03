#include "../includes/stone.hpp"

class Stone {
private:
    Color color;

public:
    Stone() : color(Color::None) {}

    Color getColor() const { return color; }
    void setColor(Color c) { color = c; }

    bool isEmpty() const { return color == Color::None; }
};
