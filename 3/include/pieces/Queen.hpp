#pragma once

#include "ChessPiece.hpp"

/**
 * @brief Queen chess piece
 *
 * Moves in straight lines and diagonals (combination of rook and bishop)
 * One portal per move allowed
 */
class Queen : public ChessPiece {
public:
    /**
     * @brief Constructs queen piece
     * @param color Piece color
     * @param position Initial board position
     */
    Queen(PieceColor color, const BoardPosition& position);

    /**
     * @brief Gets all possible moves for this queen
     * @param board Game board pointer
     * @return Vector of possible moves
     */
    std::vector<Move> getPossibleMoves(GameBoard* board) const override;

    /**
     * @brief Validates if a move is legal for this queen
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
};

