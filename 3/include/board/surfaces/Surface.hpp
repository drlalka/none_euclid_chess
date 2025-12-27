#pragma once

#include <map>
#include <vector>
#include <utility>
#include "../../common/Enums.hpp"
#include "../Cell.hpp"
#include "../PortalLink.hpp"

class ChessPiece;

/**
 * @brief Abstract base class representing a game surface
 *
 * Surface manages a 2D grid of cells and provides methods for piece placement,
 * portal management, and board initialization. Different surface types
 * (rectangular, cylindrical, spherical) define different topologies.
 */
class Surface {
protected:
    int surfaceId;
    SurfaceType surfaceType;
    std::map<std::pair<int, int>, Cell*> cells;
    std::map<std::pair<int, int>, PortalLink*> portals;

public:
    /**
     * @brief Constructs a surface with specified ID and type
     * @param surfaceId Unique identifier for this surface
     * @param type Type of surface (rectangular, cylindrical, spherical)
     */
    Surface(int surfaceId, SurfaceType type);

    virtual ~Surface();

    /**
     * @brief Initializes the board by creating cells and linking neighbors
     *
     * Must be implemented by derived classes to define surface topology
     */
    virtual void initializeBoard() = 0;

    /**
     * @brief Sets up initial piece positions on the surface
     *
     * Must be implemented by derived classes to place pieces according
     * to the game rules for this surface type
     */
    virtual void setupInitialPosition() = 0;

    /**
     * @brief Checks if coordinates are valid for this surface
     * @param x X coordinate
     * @param y Y coordinate
     * @return true if coordinates are within surface bounds
     */
    virtual bool isValidCoordinate(int x, int y) const = 0;

    /**
     * @brief Gets cell at specified coordinates
     * @param x X coordinate
     * @param y Y coordinate
     * @return Pointer to cell or nullptr if not found
     */
    Cell* getCell(int x, int y);

    /**
     * @brief Gets piece at specified coordinates
     * @param x X coordinate
     * @param y Y coordinate
     * @return Pointer to piece or nullptr if cell is empty
     */
    ChessPiece* getPieceAt(int x, int y) const;

    /**
     * @brief Adds a portal linking two positions
     * @param fromX Source X coordinate
     * @param fromY Source Y coordinate
     * @param toSurfaceId Target surface ID
     * @param toX Target X coordinate
     * @param toY Target Y coordinate
     */
    void addPortal(int fromX, int fromY, int toSurfaceId, int toX, int toY);

    /**
     * @brief Removes portal at specified position
     * @param x X coordinate
     * @param y Y coordinate
     */
    void deletePortal(int x, int y);

    /**
     * @brief Places a piece on the surface
     * @param piece Unique pointer to piece to place
     * @param x X coordinate
     * @param y Y coordinate
     */
    void addPiece(std::unique_ptr<ChessPiece> piece, int x, int y);

    /**
     * @brief Removes piece from specified position
     * @param x X coordinate
     * @param y Y coordinate
     * @return Unique pointer to removed piece or nullptr
     */
    std::unique_ptr<ChessPiece> removePiece(int x, int y);

    /**
     * @brief Gets all pieces on this surface
     * @return Vector of raw pointers to all pieces
     */
    std::vector<ChessPiece*> getAllPieces() const;

    /**
     * @brief Gets surface identifier
     * @return Surface ID
     */
    int getSurfaceId() const;

    /**
     * @brief Gets width of the surface
     * @return Width
     */
    virtual int getWidth() const = 0;

    /**
     * @brief Gets height of the surface
     * @return Height
     */
    virtual int getHeight() const = 0;
};


