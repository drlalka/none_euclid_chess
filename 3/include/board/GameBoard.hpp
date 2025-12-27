#pragma once

#include <vector>
#include <memory>
#include "surfaces/Surface.hpp"
#include "../common/BoardPosition.hpp"
#include "../common/Move.hpp"

class ChessPiece;

/**
 * @brief Manages collection of surfaces and piece movement
 */
class GameBoard {
private:
    std::vector<std::unique_ptr<Surface>> surfaces;

public:
    /**
     * @brief Default constructor
     */
    GameBoard();
    ~GameBoard() = default;

    /**
     * @brief Gets surface by ID
     * @param id Surface identifier
     * @return Raw pointer to surface or nullptr
     */
    Surface* getSurface(int id) const;

    /**
     * @brief Moves piece according to move
     * @param move Move to execute
     * @return Unique pointer to captured piece (if any)
     */
    std::unique_ptr<ChessPiece> movePiece(Move& move);

    /**
     * @brief Validates if move is possible
     * @param from Source position
     * @param to Target position
     * @return true if move is valid
     */
    bool isValidMove(const BoardPosition& from, const BoardPosition& to) const;

    /**
     * @brief Gets piece at position
     * @param pos Board position
     * @return Raw pointer to piece or nullptr
     */
    ChessPiece* getPieceAt(const BoardPosition& pos) const;

    /**
     * @brief Adds surface to board
     * @param surface Unique pointer to surface
     */
    void addSurface(std::unique_ptr<Surface> surface);

    /**
     * @brief Gets all pieces on board
     * @return Vector of raw pointers to all pieces
     */
    std::vector<ChessPiece*> getAllPieces() const;
};
