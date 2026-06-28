#pragma once

#include "ChessPiece.hpp"

/**
 * @brief Bishop chess piece
 *
 * Moves diagonally any number of squares
 * One portal per move allowed
 */
class Bishop : public ChessPiece {
public:
    /**
     * @brief Constructs bishop piece
     * @param color Piece color
     * @param position Initial position
     */
    Bishop(PieceColor color, const BoardPosition& position);

    /**
     * @brief Gets all possible moves for bishop
     * @param board Game board pointer
     * @return Vector of possible moves
     */
    std::vector<Move> getPossibleMoves(GameBoard* board) const override;

    /**
     * @brief Validates if move is legal for bishop
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

