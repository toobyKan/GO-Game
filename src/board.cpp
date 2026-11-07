#include <algorithm>
#include <stdexcept>

#include "board.hpp"

Board::Board(int size)
    : grid_(), size_(size) {
    if (size <= 0) {
        throw std::invalid_argument("Board size must be positive");
    }

    grid_.assign(static_cast<std::size_t>(size) * size, Stone::None);
}

Stone Board::getStoneAt(int x, int y) const {
    if (x >= 0 && x < size_ && y >=0 && y < size_) {
        return grid_[y * size_ + x];
    }
    return Stone::None;
}

int Board::getSize() const {
    return size_;
}

void Board::setStoneAt(int x, int y, Stone stone) {
    if (x >= 0 && x < size_ && y >=0 && y < size_) {
        grid_[y * size_ + x] = stone;
    }
}

void Board::restoreState(const std::vector<Stone>& state) {
    if (state.size() == grid_.size()) {
        grid_ = state;
    }
}

std::vector<Stone> Board::getBoardState() const {
    return grid_;
}