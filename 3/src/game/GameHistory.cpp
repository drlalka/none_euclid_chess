#include "../../include/game/GameHistory.hpp"
#include "../../include/pieces/ChessPiece.hpp"

GameHistory::GameHistory() {
}

GameHistory::~GameHistory() {
    // Captured pieces are managed by GameManager
}

void GameHistory::addMove(const Move& move, ChessPiece* capturedPiece) {
    moves.push_back(move);
    if (capturedPiece != nullptr) {
        capturedPieces.push_back(capturedPiece);
    }
}

Move GameHistory::getLastMove() const {
    if (moves.empty()) {
        throw std::runtime_error("No moves in history");
    }
    return moves.back();
}

void GameHistory::undoLastMove() {
    if (!moves.empty()) {
        moves.pop_back();
    }
    if (!capturedPieces.empty()) {
        capturedPieces.pop_back();
    }
}

const std::vector<Move>& GameHistory::getMoves() const {
    return moves;
}

void GameHistory::clear() {
    moves.clear();
    capturedPieces.clear();
}

