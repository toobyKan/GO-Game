#include "board_view.hpp"

BoardView::BoardView(const Board& board, int window_width, int window_height) 
    : board_(board), 
    geometry_(board.getSize(), window_width, window_height) {}

void BoardView::setWindowSize(int window_width, int window_height) {
    geometry_.resize(window_width, window_height);
}

void BoardView::draw(IGraphicsRenderer& renderer) const {
    drawBoardGrid(renderer);
    drawStones(renderer);
}

void BoardView::drawBoardGrid(IGraphicsRenderer& renderer) const {
     renderer.setColor(0, 0, 0, 255);

    for (int i = 0; i < board_.getSize(); ++i) {
        const int x = geometry_.gridToPixelX(i);
        const int y = geometry_.gridToPixelY(i);

        renderer.drawLine(x, geometry_.getBoardStartY(), x, geometry_.getBoardEndY());
        renderer.drawLine(geometry_.getBoardStartX(), y, geometry_.getBoardEndX(), y);
    }
}

void BoardView::drawStones(IGraphicsRenderer& renderer) const {
    for (int x = 0; x < board_.getSize(); ++x) {
        for (int y = 0; y < board_.getSize(); ++y) {
            Stone stone = board_.getStoneAt(x, y);
            if (stone != Stone::None) {
                drawStone(renderer, x, y, stone);
            }
        }
    }
}

void BoardView::drawStone(IGraphicsRenderer& renderer, int x, int y, Stone stone) const {
    if (stone == Stone::Black) {
        renderer.setColor(0, 0, 0, 255);  // Black
    } else {
        renderer.setColor(255, 255, 255, 255);  // White
    }

    const int stone_x = geometry_.gridToPixelX(x);
    const int stone_y = geometry_.gridToPixelY(y);
    const int radius = geometry_.getStoneRadius();

    renderer.drawFilledCircle(stone_x, stone_y, radius);
}