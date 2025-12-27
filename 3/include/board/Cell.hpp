#pragma once

#include "../common/BoardPosition.hpp"
#include "PortalLink.hpp"
#include <array>
#include <memory>

class ChessPiece;

/**
 * @brief Represents single cell on chess board
 *
 * Cell can contain a piece, portal link, and references to 8 neighbor cells
 */
class Cell {
public:
    /**
     * @brief Directions to neighbor cells
     */
    enum Direction {
        NORTH = 0,      ///< Up
        NORTHEAST = 1,  ///< Up-right diagonal
        EAST = 2,       ///< Right
        SOUTHEAST = 3,  ///< Down-right diagonal
        SOUTH = 4,      ///< Down
        SOUTHWEST = 5,  ///< Down-left diagonal
        WEST = 6,       ///< Left
        NORTHWEST = 7   ///< Up-left diagonal
    };

private:
    BoardPosition position;
    std::unique_ptr<ChessPiece> piece;
    std::unique_ptr<PortalLink> portalLink;
    std::array<Cell*, 8> neighbors;

public:
    /**
     * @brief Constructs cell at position
     * @param pos Board position
     */
    Cell(const BoardPosition& pos);
    ~Cell() = default;

    /**
     * @brief Checks if cell is empty
     * @return true if no piece present
     */
    bool isEmpty() const { return piece == nullptr; }

    /**
     * @brief Checks if cell has portal
     * @return true if portal link exists
     */
    bool hasPortal() const { return portalLink != nullptr; }

    /**
     * @brief Gets piece in cell
     * @return Raw pointer to piece or nullptr
     */
    ChessPiece* getPiece() const { return piece.get(); }

    /**
     * @brief Gets portal link
     * @return Raw pointer to portal or nullptr
     */
    PortalLink* getPortalLink() const { return portalLink.get(); }

    /**
     * @brief Gets cell position
     * @return Board position
     */
    BoardPosition getPosition() const { return position; }

    /**
     * @brief Places piece in cell
     * @param p Unique pointer to piece
     */
    void setPiece(std::unique_ptr<ChessPiece> p);

    /**
     * @brief Removes piece from cell
     * @return Unique pointer to removed piece
     */
    std::unique_ptr<ChessPiece> removePiece();

    /**
     * @brief Sets portal link
     * @param link Unique pointer to portal link
     */
    void setPortalLink(std::unique_ptr<PortalLink> link);

    /**
     * @brief Gets portal destination position
     * @return Destination position
     */
    BoardPosition getPortalDestination() const;

    /**
     * @brief Sets neighbor cell in direction
     * @param dir Direction
     * @param cell Pointer to neighbor cell
     */
    void setNeighbor(Direction dir, Cell* cell);

    /**
     * @brief Gets neighbor cell in direction
     * @param dir Direction
     * @return Pointer to neighbor or nullptr
     */
    Cell* getNeighbor(Direction dir) const;
};

