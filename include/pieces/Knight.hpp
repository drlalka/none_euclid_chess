#pragma once

#include "ChessPiece.hpp"

/**
 * @brief Knight chess piece
 *
 * Moves in L-shape pattern (2 squares in one direction, 1 in perpendicular)
 * Two portals plus one cell movement allowed
 */
class Knight : public ChessPiece {
public:
    /**
     * @brief Constructs knight piece
     * @param color Piece color
     * @param position Initial position
     */
    Knight(PieceColor color, const BoardPosition& position);

    /**
     * @brief Gets all possible moves for knight
     * @param board Game board pointer
     * @return Vector of possible moves
     */
    std::vector<Move> getPossibleMoves(GameBoard* board) const override;

    /**
     * @brief Validates if move is legal for knight
     * @param move Move to validate
     * @param board Game board pointer
     * @return true if move is valid
     */
    bool isValidMove(const Move& move, GameBoard* board) const override;

    /**
     * @brief Gets maximum portal usage
     * @return 2 (two portals per move)
     */
    int getMaxPortalUse() const override;
};

