#pragma once

#include "BoardPosition.hpp"
#include "Enums.hpp"

/**
 * @brief Represents a chess move with metadata
 */
class Move {
private:
    BoardPosition from;
    BoardPosition to;
    long timestamp;
    bool portalUsed;
    PieceColor movingPieceColor;
    PieceType capturedType;
    bool hadCapture;
    bool pieceHadMoved;

public:
    /**
     * @brief Constructs a move
     * @param from Starting position
     * @param to Target position
     * @param portalUsed Whether move uses portal
     */
    Move(const BoardPosition& from, const BoardPosition& to, bool portalUsed = false);

    /**
     * @brief Gets starting position
     * @return Source position
     */
    const BoardPosition& getFrom() const;

    /**
     * @brief Gets target position
     * @return Destination position
     */
    const BoardPosition& getTo() const;

    /**
     * @brief Gets move timestamp
     * @return Timestamp in milliseconds
     */
    long getTimestamp() const;

    /**
     * @brief Checks if move uses portal
     * @return true if portal was used
     */
    bool isPortalUsed() const;

    /**
     * @brief Sets color of moving piece
     * @param color Piece color
     */
    void setMovingPieceColor(PieceColor color);

    /**
     * @brief Gets color of moving piece
     * @return Piece color
     */
    PieceColor getMovingPieceColor() const;

    /**
     * @brief Sets type of captured piece
     * @param type Piece type
     */
    void setCapturedType(PieceType type);

    /**
     * @brief Gets type of captured piece
     * @return Piece type
     */
    PieceType getCapturedType() const;

    /**
     * @brief Checks if move captured a piece
     * @return true if piece was captured
     */
    bool hadPieceCapture() const;

    /**
     * @brief Sets whether piece had moved before
     * @param moved true if piece had moved
     */
    void setPieceHadMoved(bool moved);

    /**
     * @brief Gets whether piece had moved before
     * @return true if piece had moved
     */
    bool getPieceHadMoved() const;

    /**
     * @brief Equality operator
     */
    bool operator==(const Move& other) const;
};


