#pragma once

#include "ChessPiece.hpp"

/**
 * @brief King chess piece
 *
 * Moves one square in any direction (8 directions)
 * One portal per move allowed
 * Most important piece - losing king means checkmate
 */
class King : public ChessPiece {
private:
    bool hasMoved;
    bool isInCheck;

public:
    /**
     * @brief Constructs king piece
     * @param color Piece color
     * @param position Initial position
     */
    King(PieceColor color, const BoardPosition& position);

    /**
     * @brief Gets all possible moves for king
     * @param board Game board pointer
     * @return Vector of possible moves
     */
    std::vector<Move> getPossibleMoves(GameBoard* board) const override;

    /**
     * @brief Validates if move is legal for king
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
     * @brief Checks if king can castle
     * @return true if castling is possible
     */
    bool canCastle() const;

    /**
     * @brief Sets whether king has moved
     * @param moved true if king moved
     */
    void setMoved(bool moved);

    /**
     * @brief Sets check status
     * @param check true if king is in check
     */
    void setCheck(bool check);

    /**
     * @brief Gets whether king has moved
     * @return true if king has moved
     */
    bool getHasMoved() const;

    /**
     * @brief Gets check status
     * @return true if king is in check
     */
    bool getIsInCheck() const;

    /**
     * @brief Called when king is moved
     * @param move Move that was made
     */
    void onMove(const Move& move) override;

    /**
     * @brief Called before move to store state
     * @param move Move object to update
     */
    void beforeMove(Move& move) const override;
};

