#pragma once

#include <vector>
#include <memory>
#include "../common/Move.hpp"

class ChessPiece;

/**
 * @brief Stores move history and captured pieces
 */
class GameHistory {
private:
    std::vector<Move> moves;
    std::vector<std::unique_ptr<ChessPiece>> capturedPieces;

public:
    GameHistory() = default;
    ~GameHistory() = default;

    /**
     * @brief Adds move to history
     * @param move Move to record
     * @param capturedPiece Piece captured during move (if any)
     */
    void addMove(const Move& move, std::unique_ptr<ChessPiece> capturedPiece = nullptr);

    /**
     * @brief Gets last move made
     * @return Last move
     * @throws std::out_of_range if no moves exist
     */
    Move getLastMove() const;

    /**
     * @brief Undoes last move
     * @return Captured piece to restore (if any)
     */
    std::unique_ptr<ChessPiece> undoLastMove();

    /**
     * @brief Gets all moves
     * @return Vector of all moves
     */
    const std::vector<Move>& getMoves() const;

    /**
     * @brief Clears all history
     */
    void clear();
};


