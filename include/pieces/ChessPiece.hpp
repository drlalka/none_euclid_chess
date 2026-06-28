#pragma once

#include <vector>
#include "../common/Enums.hpp"
#include "../common/BoardPosition.hpp"
#include "../common/Move.hpp"

class GameBoard;

/**
 * @brief Abstract base class for all chess pieces
 */
class ChessPiece {
protected:
    PieceColor color;
    PieceType type;
    BoardPosition position;

public:
    /**
     * @brief Constructs chess piece
     * @param color Piece color
     * @param type Piece type
     * @param position Initial position
     */
    ChessPiece(PieceColor color, PieceType type, const BoardPosition& position);
    virtual ~ChessPiece() = default;

    /**
     * @brief Gets all possible moves for this piece
     * @param board Game board pointer
     * @return Vector of possible moves
     */
    virtual std::vector<Move> getPossibleMoves(GameBoard* board) const = 0;

    /**
     * @brief Validates if move is legal for this piece
     * @param move Move to validate
     * @param board Game board pointer
     * @return true if move is valid
     */
    virtual bool isValidMove(const Move& move, GameBoard* board) const = 0;

    /**
     * @brief Gets maximum portal usage for this piece
     * @return Max portal count (-1 for unlimited)
     */
    virtual int getMaxPortalUse() const = 0;

    /**
     * @brief Gets piece type
     * @return Piece type enum
     */
    PieceType getType() const;

    /**
     * @brief Gets piece value (for scoring)
     * @return Numeric value
     */
    int getValue() const;

    /**
     * @brief Gets piece color
     * @return Piece color enum
     */
    PieceColor getColor() const;

    /**
     * @brief Gets current position
     * @return Board position
     */
    const BoardPosition& getPosition() const;

    /**
     * @brief Moves piece to new position
     * @param newPosition Target position
     */
    void moveTo(const BoardPosition& newPosition);

    /**
     * @brief Called when piece is moved (observer pattern)
     * @param move Move that was made
     */
    virtual void onMove(const Move& move) {}

    /**
     * @brief Called before move to store state in Move object
     * @param move Move object to update
     */
    virtual void beforeMove(Move& move) const {}
};


