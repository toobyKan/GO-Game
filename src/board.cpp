#include <algorithm>

#include "../headers/board.hpp"


Board::Board(int size) : grid_(size * size, Stone::None), size_(size) {}

Stone Board::getStoneAt(int x, int y) const {
    if (x < size_ && y < size_) {
        return grid_[y * size_ + x];
    }
    return Stone::None;
}

int Board::getSize() const {
    return size_;
}

void Board::setStoneAt(int x, int y, Stone stone) {
    if (x < size_ && y < size_) {
        grid_[y * size_ + x] = stone;
    }
}

std::vector<Stone> Board::getBoardState() const {
    return grid_;
}