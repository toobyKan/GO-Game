#include <algorithm>

#include "../headers/board.hpp"


Board::Board(int size) : grid_(size, std::vector<Stone>(size, Stone::None)), size_(size) {}

Stone Board::getStoneAt(int x, int y) const {
    if (x >= 0 && x < size_ && y >= 0 && y < size_) {
        return grid_[x][y];
    }
    return Stone::None;
}

int Board::getSize() const {
    return size_;
}

void Board::setStoneAt(int x, int y, Stone stone) {
    if (x >= 0 && x < size_ && y >= 0 && y < size_) {
        grid_[x][y] = stone;
    }
}

std::vector<std::vector<Stone>> Board::getBoardState() const {
    return grid_;
}