#ifndef BOARD_VIEW_H
#define BOARD_VIEW_H

#include "drawable_entity.hpp"
#include "board.hpp"

class BoardView : public Drawable_Entity {
public:
    BoardView(const Board& board, int window_size);
    void draw(IGraphicsRenderer& renderer) const override;

private:
    void drawBoardGrid(IGraphicsRenderer& renderer) const;
    void drawStones(IGraphicsRenderer& renderer) const;
    void drawStone(IGraphicsRenderer& renderer, int x, int y, Stone stone) const;

    const Board& board_;
    int window_size_;
    int tile_size_;
};

#endif