#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "observer.hpp"

enum class Stone { None, Black, White };

class Board {
public:
    Board(int size);
    Stone getStoneAt(int x, int y) const;
    
    void setStoneAt(int x, int y, Stone stone);
    
    // Save the board state (for ko checking)
    std::vector<std::vector<Stone>> getBoardState() const;
    std::vector<std::vector<Stone>> getPreviousBoardState() const;
    void saveBoardState();

    int getSize() const;  // Make sure this method is declared
    
    void attachObserver(Observer* observer);
    void detachObserver(Observer* observer);
    void notifyObservers();

private:
    std::vector<std::vector<Stone>> grid_;
    std::vector<Observer*> observers_;
    std::vector<std::vector<Stone>> previous_state_;  // For ko rule
    int size_;
};

#endif
