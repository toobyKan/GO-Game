#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "board.hpp"
#include "move_result.hpp"

#include <set>
#include <functional>
#include <vector>

class GameLogic {
public:
    GameLogic(Board& board);
    
    MoveResult placeStone(int x, int y);
    
    Stone getCurrentPlayer() const;
private:
    void captureStones(int x, int y, Stone stone);
    
    bool isSuicide(int x, int y, Stone stone);    
    bool hasLiberties(int x, int y, Stone stone, std::set<std::pair<int, int>>& visited);
    bool violatesKoRule();

    void switchPlayer();

    Board& board_;
    Stone currentPlayer_;
    std::vector<Stone> previousState_;
    
    std::vector<std::pair<int, int>> getNeighbors(int x, int y) const;
    
};

#endif