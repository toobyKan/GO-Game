#ifndef BOARD_H
#define BOARD_H

#include <vector>

enum class Stone { None, Black, White };

class Board {
public:
    Board(int size);
    Stone getStoneAt(int x, int y) const;
    
    void setStoneAt(int x, int y, Stone stone);
    
    std::vector<std::vector<Stone>> getBoardState() const;

    int getSize() const;

private:
    std::vector<std::vector<Stone>> grid_;
    int size_;
};

#endif
