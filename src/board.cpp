#include <algorithm>

#include "../headers/board.hpp"
#include "../headers/observer.hpp"



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

void Board::attachObserver(Observer* observer) {
    observers_.push_back(observer);
}

void Board::detachObserver(Observer* observer) {
    observers_.erase(std::remove(observers_.begin(), observers_.end(), observer), observers_.end());
}

void Board::notifyObservers() {
    for (Observer* observer : observers_) {
        observer->update();
    }
}


void Board::setStoneAt(int x, int y, Stone stone) {
    if (x >= 0 && x < size_ && y >= 0 && y < size_) {
        grid_[x][y] = stone;
    }
}

std::vector<std::vector<Stone>> Board::getBoardState() const {
    return grid_;
}

std::vector<std::vector<Stone>> Board::getPreviousBoardState() const {
    return previous_state_;
}

void Board::saveBoardState() {
    previous_state_ = grid_;
}