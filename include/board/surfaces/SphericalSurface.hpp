#pragma once

#include "Surface.hpp"

/**
 * @brief Spherical surface with pole topology
 *
 * Represents a sphere where:
 * - LATITUDE x LONGITUDE grid (8x8)
 * - Poles at y=0 (south) and y=7 (north)
 * - Moving through a pole causes 180-degree rotation in longitude
 * - Longitude wraps around (x: 0->7->0)
 */
class SphericalSurface : public Surface {
private:
    static constexpr int LATITUDE = 8;
    static constexpr int LONGITUDE = 8;

public:
    /**
     * @brief Constructs spherical surface
     * @param surfaceId Unique surface identifier
     */
    SphericalSurface(int surfaceId);

    /**
     * @brief Initializes board with sphere topology (poles with 180° rotation)
     */
    void initializeBoard() override;

    /**
     * @brief Sets up initial chess piece positions
     */
    void setupInitialPosition() override;

    /**
     * @brief Validates coordinates within sphere bounds
     * @param x Longitude coordinate (0-7)
     * @param y Latitude coordinate (0-7)
     * @return true if coordinates are valid
     */
    bool isValidCoordinate(int x, int y) const override;

    /**
     * @brief Gets longitude dimension
     * @return Width (8)
     */
    int getWidth() const override { return LONGITUDE; }

    /**
     * @brief Gets latitude dimension
     * @return Height (8)
     */
    int getHeight() const override { return LATITUDE; }
};

