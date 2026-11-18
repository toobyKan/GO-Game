#include "game_logic.hpp"

GameLogic::GameLogic(Board& board) : board_(board), currentPlayer_(Stone::Black),
    previousState_(board.getBoardState()) {}

bool GameLogic::placeStone(int x, int y) {
    // Validate move
    if (
        x < 0 || x >= board_.getSize() ||
        y < 0 || y >= board_.getSize()
    ) {
        return false;
    }
    

    // Check if the position is empty
    if (board_.getStoneAt(x, y) != Stone::None) {
        return false;
    }

    auto boardStateBeforeMove = board_.getBoardState();
    
    // Temporarily place the stone
    board_.setStoneAt(x, y, currentPlayer_);

        // Capture opponent stones
    captureStones(x, y, currentPlayer_);
    
    // Check for suicide
    if (isSuicide(x, y, currentPlayer_)) {
        board_.restoreState(boardStateBeforeMove);  // Undo the move
        return false;
    }
    
    // Check for ko rule
    if (violatesKoRule()) {
        board_.restoreState(boardStateBeforeMove);  // Undo the move
        return false;
    }

    // Save the board state for future ko checking
    previousState_ = boardStateBeforeMove;
    switchPlayer();

    // Trigger all bound callbacks
    for (const auto& callback : onTurnCompleted) {
        if (callback) {
            callback();
        }
    }

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
    return board_.getBoardState() == previousState_;
}

std::vector<std::pair<int, int>> GameLogic::getNeighbors(int x, int y) const {
    std::vector<std::pair<int, int>> neighbors;

    if (x > 0) neighbors.push_back({x - 1, y});  // Left
    if (x < board_.getSize() - 1) neighbors.push_back({x + 1, y});  // Right
    if (y > 0) neighbors.push_back({x, y - 1});  // Up
    if (y < board_.getSize() - 1) neighbors.push_back({x, y + 1});  // Down

    return neighbors;
}

void GameLogic::switchPlayer(){
    currentPlayer_ = (currentPlayer_ == Stone::Black) ? Stone::White : Stone::Black;
}

Stone GameLogic::getCurrentPlayer() const{
    return currentPlayer_;
}