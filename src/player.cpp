#include "../includes/player.hpp"

class Player {
private:
    Color color;

public:
    Player(Color color) : color(color) {}

    Color getColor() const { return color; }

    bool makeMove(Board& board, int x, int y) {
        return board.placeStone(x, y, color);
    }
};
