#include "../headers/board_geometry.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

BoardGeometry::BoardGeometry(
    int board_size,
    int window_width,
    int window_height
)
    : board_size_(board_size),
      window_width_(0),
      window_height_(0),
      board_pixels_(0),
      origin_x_(0),
      origin_y_(0),
      spacing_(0.0) {
    if (board_size <= 0) {
        throw std::invalid_argument(
            "Board size must be positive"
        );
    }

    resize(window_width, window_height);
}

void BoardGeometry::resize(
    int window_width,
    int window_height
) {
    if (window_width <= 0 || window_height <= 0) {
        return;
    }

    window_width_ = window_width;
    window_height_ = window_height;

    // Keep the board square inside the window.
    board_pixels_ =
        std::min(window_width_, window_height_);

    // Center the square board.
    origin_x_ =
        (window_width_ - board_pixels_) / 2;

    origin_y_ =
        (window_height_ - board_pixels_) / 2;

    if (board_size_ > 1) {
        spacing_ =
            static_cast<double>(board_pixels_ - 1) /
            static_cast<double>(board_size_ - 1);
    } else {
        spacing_ = 0.0;
    }
}

int BoardGeometry::coordinateToPixel(int coordinate) const {
    return static_cast<int>(
        std::lround(
            static_cast<double>(coordinate) * spacing_
        )
    );
}

int BoardGeometry::gridToPixelX(int grid_x) const {
    return origin_x_ + coordinateToPixel(grid_x);
}

int BoardGeometry::gridToPixelY(int grid_y) const {
    return origin_y_ + coordinateToPixel(grid_y);
}

int BoardGeometry::getBoardStartX() const {
    return origin_x_;
}

int BoardGeometry::getBoardStartY() const {
    return origin_y_;
}

int BoardGeometry::getBoardEndX() const {
    return origin_x_ + board_pixels_ - 1;
}

int BoardGeometry::getBoardEndY() const {
    return origin_y_ + board_pixels_ - 1;
}

int BoardGeometry::getStoneRadius() const {
    if (board_size_ <= 1) {
        return board_pixels_ / 2;
    }

    return static_cast<int>(spacing_ * 0.45);
}

bool BoardGeometry::screenToGrid(
    int mouse_x,
    int mouse_y,
    int& grid_x,
    int& grid_y
) const {
    if (mouse_x < getBoardStartX() ||
        mouse_x > getBoardEndX() ||
        mouse_y < getBoardStartY() ||
        mouse_y > getBoardEndY()) {
        return false;
    }

    if (board_size_ == 1) {
        grid_x = 0;
        grid_y = 0;
        return true;
    }

    grid_x = static_cast<int>(
        std::lround(
            static_cast<double>(
                mouse_x - getBoardStartX()
            ) / spacing_
        )
    );

    grid_y = static_cast<int>(
        std::lround(
            static_cast<double>(
                mouse_y - getBoardStartY()
            ) / spacing_
        )
    );

    return grid_x >= 0 &&
           grid_x < board_size_ &&
           grid_y >= 0 &&
           grid_y < board_size_;
}