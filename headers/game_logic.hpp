#include "board.hpp"
#include <set>

class GameLogic {
public:
    GameLogic(Board& board);
    
    bool placeStone(int x, int y, Stone stone);
    
private:
    void captureStones(int x, int y, Stone stone);
    
    bool isSuicide(int x, int y, Stone stone);    
    bool hasLiberties(int x, int y, Stone stone, std::set<std::pair<int, int>>& visited);
    bool violatesKoRule();

    std::vector<std::pair<int, int>> getNeighbors(int x, int y) const;

    Board& board_;  // Reference to the board
};
