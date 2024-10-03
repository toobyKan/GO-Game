#ifndef STONE_H
#define STONE_H

enum Color {
    None,
    White,
    Black
};

class Stone{
public:
    Stone();

    Color getColor() const;
    void setColor(Color);

    bool isEmpty() const;

private:
    Color color;
};

#endif