#ifndef BOARD_VIEW_H
#define BOARD_VIEW_H

#include "drawable_entity.hpp"
#include "board_geometry.hpp"
#include "board.hpp"

class BoardView : public Drawable_Entity {
public:
    BoardView(const Board& board, int window_width, int window_height);
    void setWindowSize(int window_width, int window_height);
    void draw(IGraphicsRenderer& renderer) const override;

private:
    void drawBoardGrid(IGraphicsRenderer& renderer) const;
    void drawStones(IGraphicsRenderer& renderer) const;
    void drawStone(IGraphicsRenderer& renderer, int x, int y, Stone stone) const;

    const Board& board_;
    BoardGeometry geometry_;
};

#endif