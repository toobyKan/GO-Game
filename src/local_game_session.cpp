#include "local_game_session.hpp"


LocalGameSession::LocalGameSession(int board_size) :  
board_(board_size), 
gameLogic_(board_) {}

bool LocalGameSession::submitMove(int x, int y) {
    return gameLogic_.placeStone(x, y);
}

const Board& LocalGameSession::getBoard() const {
    return board_;
}

Stone LocalGameSession::getCurrentPlayer() const {
    return gameLogic_.getCurrentPlayer();
}

void LocalGameSession::update(double) {}