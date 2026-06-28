#pragma once

#include "ChessPiece.hpp"

/**
 * @brief Rook chess piece
 *
 * Moves in straight lines (vertical/horizontal)
 * Unlimited portals allowed
 */
class Rook : public ChessPiece {
private:
    bool hasMoved;

public:
    /**
     * @brief Constructs rook piece
     * @param color Piece color
     * @param position Initial board position
     */
    Rook(PieceColor color, const BoardPosition& position);

    /**
     * @brief Gets all possible moves for this rook
     * @param board Game board pointer
     * @return Vector of possible moves
     */
    std::vector<Move> getPossibleMoves(GameBoard* board) const override;

    /**
     * @brief Validates if a move is legal for this rook
     * @param move Move to validate
     * @param board Game board pointer
     * @return true if move is valid
     */
    bool isValidMove(const Move& move, GameBoard* board) const override;

    /**
     * @brief Gets maximum portal usage
     * @return -1 (unlimited portals)
     */
    int getMaxPortalUse() const override;

    /**
     * @brief Sets whether rook has moved (for castling)
     * @param moved true if rook has moved
     */
    void setMoved(bool moved);

    /**
     * @brief Checks if rook has moved
     * @return true if rook has moved
     */
    bool getHasMoved() const;

    /**
     * @brief Called when rook is moved
     * @param move Move that was made
     */
    void onMove(const Move& move) override;

    /**
     * @brief Called before move to store state
     * @param move Move object to update
     */
    void beforeMove(Move& move) const override;
};

