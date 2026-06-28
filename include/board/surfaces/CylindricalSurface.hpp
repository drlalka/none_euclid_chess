#pragma once

#include "Surface.hpp"

/**
 * @brief Cylindrical surface with wraparound edges
 *
 * Represents a cylinder where:
 * - Fixed circumference (8 cells)
 * - Variable height
 * - X-axis wraps around (0->7->0)
 * - Y-axis has edges (no wraparound)
 */
class CylindricalSurface : public Surface {
private:
    static constexpr int CIRCUMFERENCE = 8;
    int height;

public:
    /**
     * @brief Constructs cylindrical surface
     * @param surfaceId Unique surface identifier
     * @param height Cylinder height (default 8)
     */
    CylindricalSurface(int surfaceId, int height = 8);

    /**
     * @brief Initializes board with cylinder topology (x-axis wraparound)
     */
    void initializeBoard() override;

    /**
     * @brief Sets up initial chess piece positions
     */
    void setupInitialPosition() override;

    /**
     * @brief Validates coordinates within cylinder bounds
     * @param x Circumference coordinate (0-7, wraps)
     * @param y Height coordinate (0 to height-1)
     * @return true if coordinates are valid
     */
    bool isValidCoordinate(int x, int y) const override;

    /**
     * @brief Gets circumference dimension
     * @return Width (8)
     */
    int getWidth() const override { return CIRCUMFERENCE; }

    /**
     * @brief Gets height dimension
     * @return Height
     */
    int getHeight() const override { return height; }
};

