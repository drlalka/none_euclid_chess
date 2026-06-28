#pragma once

#include "Surface.hpp"

/**
 * @brief Standard flat chess board with custom dimensions
 *
 * Represents a flat rectangular board with:
 * - Custom width (must be even, >= 8)
 * - Custom height (must be even, >= 8)
 * - No wraparound on any axis
 * - Standard chess board topology (8 neighbors per cell)
 */
class StandardSurface : public Surface {
private:
    int width;
    int height;

public:
    /**
     * @brief Constructs standard surface with custom dimensions
     * @param surfaceId Unique surface identifier
     * @param width Board width (must be even and >= 8)
     * @param height Board height (must be even and >= 8)
     * @throws std::invalid_argument if dimensions are invalid
     */
    StandardSurface(int surfaceId, int width, int height);

    /**
     * @brief Initializes board with standard flat topology
     */
    void initializeBoard() override;

    /**
     * @brief Sets up initial chess piece positions
     *
     * White pieces on bottom two rows (y=0,1)
     * Black pieces on top two rows (y=height-2, height-1)
     */
    void setupInitialPosition() override;

    /**
     * @brief Validates coordinates within bounds
     * @param x X coordinate (0 to width-1)
     * @param y Y coordinate (0 to height-1)
     * @return true if coordinates are valid
     */
    bool isValidCoordinate(int x, int y) const override;

    /**
     * @brief Gets width dimension
     * @return Width
     */
    int getWidth() const override { return width; }

    /**
     * @brief Gets height dimension
     * @return Height
     */
    int getHeight() const override { return height; }

    ~StandardSurface() override = default;
};

