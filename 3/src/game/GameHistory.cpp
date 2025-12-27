#include "../../include/game/GameHistory.hpp"
#include "../../include/pieces/ChessPiece.hpp"
#include <stdexcept>

void GameHistory::addMove(const Move& move, std::unique_ptr<ChessPiece> capturedPiece) {
    moves.push_back(move);
    if (capturedPiece != nullptr) {
        capturedPieces.push_back(std::move(capturedPiece));
    } else {
        capturedPieces.push_back(nullptr);
    }
}

Move GameHistory::getLastMove() const {
    if (moves.empty()) {
        throw std::runtime_error("No moves in history");
    }
    return moves.back();
}

std::unique_ptr<ChessPiece> GameHistory::undoLastMove() {
    if (moves.empty()) {
        return nullptr;
    }

    moves.pop_back();

    if (!capturedPieces.empty()) {
        auto captured = std::move(capturedPieces.back());
        capturedPieces.pop_back();
        return captured;
    }

    return nullptr;
}

const std::vector<Move>& GameHistory::getMoves() const {
    return moves;
}

void GameHistory::clear() {
    moves.clear();
    capturedPieces.clear();
}

