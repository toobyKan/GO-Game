#include "../headers/board_view.hpp"

BoardView::BoardView(const Board& board, int window_size) 
    : board_(board), window_size_(window_size) {
    tile_size_ = window_size_ / board_.getSize();
}

void BoardView::draw(IGraphicsRenderer& renderer) const {
    drawBoardGrid(renderer);
    drawStones(renderer);
}

void BoardView::drawBoardGrid(IGraphicsRenderer& renderer) const {
    renderer.setColor(0, 0, 0, 255);  // Black color for lines

    // Draw vertical lines
    for (int i = 0; i < board_.getSize(); ++i) {
        int x = i * tile_size_;
        renderer.drawLine(x, 0, x, window_size_);
    }

    // Draw horizontal lines
    for (int i = 0; i < board_.getSize(); ++i) {
        int y = i * tile_size_;
        renderer.drawLine(0, y, window_size_, y);
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

    int stone_x = x * tile_size_ + tile_size_ / 2;
    int stone_y = y * tile_size_ + tile_size_ / 2;
    int radius = tile_size_ / 3;

    renderer.drawFilledCircle(stone_x, stone_y, radius);
}