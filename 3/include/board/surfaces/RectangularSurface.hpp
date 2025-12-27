#pragma once

#include "Surface.hpp"

/**
 * @brief Standard rectangular chess board surface
 *
 * Represents a flat rectangular board:
 * - Fixed width (8 cells)
 * - Variable height
 * - No wraparound on any axis
 * - Standard chess board topology
 */
class RectangularSurface : public Surface {
private:
    static constexpr int WIDTH = 8;
    int height;

public:
    /**
     * @brief Constructs rectangular surface
     * @param surfaceId Unique surface identifier
     * @param height Board height (default 8)
     */
    RectangularSurface(int surfaceId, int height = 8);

    /**
     * @brief Initializes board with standard rectangular topology
     */
    void initializeBoard() override;

    /**
     * @brief Sets up initial chess piece positions (standard layout)
     */
    void setupInitialPosition() override;

    /**
     * @brief Validates coordinates within rectangular bounds
     * @param x X coordinate (0-7)
     * @param y Y coordinate (0 to height-1)
     * @return true if coordinates are valid
     */
    bool isValidCoordinate(int x, int y) const override;

    /**
     * @brief Gets width dimension
     * @return Width (8)
     */
    int getWidth() const override { return WIDTH; }

    /**
     * @brief Gets height dimension
     * @return Height
     */
    int getHeight() const override { return height; }
};

