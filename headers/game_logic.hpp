#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "board.hpp"

#include <set>
#include <functional>

class GameLogic {
public:
    GameLogic(Board& board);
    std::vector<std::function<void()>> onTurnCompleted;
    
    bool placeStone(int x, int y, Stone stone);
    
    Stone getCurrentPlayer();
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