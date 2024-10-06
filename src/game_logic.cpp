#include "../headers/game_logic.hpp"

GameLogic::GameLogic(Board& board) : board_(board) {}

bool GameLogic::placeStone(int x, int y, Stone stone) {
    // Check if the position is empty
    if (board_.getStoneAt(x, y) != Stone::None) {
        return false;
    }
    
    // Temporarily place the stone
    board_.setStoneAt(x, y, stone);
    
    // Check for suicide
    if (isSuicide(x, y, stone)) {
        board_.setStoneAt(x, y, Stone::None);  // Undo the move
        return false;
    }
    
    // Capture opponent stones
    captureStones(x, y, stone);
    
    // Check for ko rule
    if (violatesKoRule()) {
        board_.setStoneAt(x, y, Stone::None);  // Undo the move
        return false;
    }

    // Save the board state for future ko checking
    board_.saveBoardState();
    
    board_.notifyObservers();

    return true;  // The move was valid
}

bool GameLogic::isSuicide(int x, int y, Stone stone) {
    std::set<std::pair<int, int>> visited;
    
    return !hasLiberties(x, y, stone, visited);
}

void GameLogic::captureStones(int x, int y, Stone stone) {
    Stone opponent = (stone == Stone::Black) ? Stone::White : Stone::Black;

    // Check the four neighboring positions for opponent stones to capture
    for (auto& neighbor : getNeighbors(x, y)) {
        int nx = neighbor.first;
        int ny = neighbor.second;
        
        if (board_.getStoneAt(nx, ny) == opponent) {
            std::set<std::pair<int, int>> visited;
            
            // If the opponent's group has no liberties, remove it
            if (!hasLiberties(nx, ny, opponent, visited)) {
                for (auto& pos : visited) {
                    board_.setStoneAt(pos.first, pos.second, Stone::None);  // Remove the captured stones
                }
            }
        }
    }
}

bool GameLogic::hasLiberties(int x, int y, Stone stone, std::set<std::pair<int, int>>& visited) {
    if (visited.count({x, y})) {
        return false;
    }
    
    visited.insert({x, y});

    for (auto& neighbor : getNeighbors(x, y)) {
        int nx = neighbor.first;
        int ny = neighbor.second;

        if (board_.getStoneAt(nx, ny) == Stone::None) {
            return true;  // Found a liberty
        }

        if (board_.getStoneAt(nx, ny) == stone && hasLiberties(nx, ny, stone, visited)) {
            return true;  // Part of the same group with liberties
        }
    }

    return false;  // No liberties found
}

bool GameLogic::violatesKoRule() {
    // Compare current board state with the previous state for ko violation
    return board_.getBoardState() == board_.getPreviousBoardState();
}

std::vector<std::pair<int, int>> GameLogic::getNeighbors(int x, int y) const {
    std::vector<std::pair<int, int>> neighbors;

    if (x > 0) neighbors.push_back({x - 1, y});  // Left
    if (x < board_.getSize() - 1) neighbors.push_back({x + 1, y});  // Right
    if (y > 0) neighbors.push_back({x, y - 1});  // Up
    if (y < board_.getSize() - 1) neighbors.push_back({x, y + 1});  // Down

    return neighbors;
}
