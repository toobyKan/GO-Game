#include "board.hpp"
#include "subject.hpp"

#include <set>

class GameLogic: public Subject {
public:
    GameLogic(Board& board);
    
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
    std::vector<std::vector<Stone>> previousState_;

    std::vector<std::pair<int, int>> getNeighbors(int x, int y) const;
    
};
