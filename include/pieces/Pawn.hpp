#pragma once

#include "ChessPiece.hpp"

/**
 * @brief Pawn chess piece
 *
 * Moves forward, captures diagonally
 * One portal per move allowed
 * Can promote upon reaching opposite end
 */
class Pawn : public ChessPiece {
private:
    bool hasMoved;

public:
    /**
     * @brief Constructs pawn piece
     * @param color Piece color
     * @param position Initial position
     */
    Pawn(PieceColor color, const BoardPosition& position);

    /**
     * @brief Gets all possible moves for pawn
     * @param board Game board pointer
     * @return Vector of possible moves
     */
    std::vector<Move> getPossibleMoves(GameBoard* board) const override;

    /**
     * @brief Validates if move is legal for pawn
     * @param move Move to validate
     * @param board Game board pointer
     * @return true if move is valid
     */
    bool isValidMove(const Move& move, GameBoard* board) const override;

    /**
     * @brief Gets maximum portal usage
     * @return 1 (one portal per move)
     */
    int getMaxPortalUse() const override;

    /**
     * @brief Checks if pawn can promote
     * @return true if pawn reached promotion rank
     */
    bool canPromote() const;

    /**
     * @brief Promotes pawn to another piece type
     * @param newType Type to promote to (Queen/Rook/Bishop/Knight)
     * @return Pointer to new piece
     */
    ChessPiece* promote(PieceType newType);

    /**
     * @brief Sets whether pawn has moved
     * @param moved true if pawn moved
     */
    void setMoved(bool moved);

    /**
     * @brief Gets whether pawn has moved
     * @return true if pawn has moved
     */
    bool getHasMoved() const;

    /**
     * @brief Called when pawn is moved
     * @param move Move that was made
     */
    void onMove(const Move& move) override;

    /**
     * @brief Called before move to store state
     * @param move Move object to update
     */
    void beforeMove(Move& move) const override;
};


