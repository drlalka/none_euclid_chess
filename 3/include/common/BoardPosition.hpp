#pragma once

#include <string>

/**
 * @brief Represents position on game board with surface ID and coordinates
 */
class BoardPosition {
private:
    int surfaceId;
    int x;
    int y;

public:
    /**
     * @brief Default constructor - creates invalid position
     */
    BoardPosition();

    /**
     * @brief Constructs position with surface and coordinates
     * @param surfaceId Surface identifier
     * @param x X coordinate
     * @param y Y coordinate
     */
    BoardPosition(int surfaceId, int x, int y);

    /**
     * @brief Gets surface ID
     * @return Surface identifier
     */
    int getSurfaceId() const { return surfaceId; }

    /**
     * @brief Gets X coordinate
     * @return X value
     */
    int getX() const { return x; }

    /**
     * @brief Gets Y coordinate
     * @return Y value
     */
    int getY() const { return y; }

    /**
     * @brief Checks equality with another position
     * @param other Other position
     * @return true if positions are equal
     */
    bool equals(const BoardPosition& other) const;

    /**
     * @brief Checks if position is valid
     * @return true if position has valid coordinates
     */
    bool isValid() const;

    /**
     * @brief Converts position to string representation
     * @return String like "1:2,3" (surface:x,y)
     */
    std::string toString() const;

    /**
     * @brief Creates position from string
     * @param str String in format "surfaceId:x,y"
     * @return BoardPosition object
     */
    static BoardPosition fromString(const std::string& str);

    /**
     * @brief Equality operator
     */
    bool operator==(const BoardPosition& other) const;

    /**
     * @brief Inequality operator
     */
    bool operator!=(const BoardPosition& other) const;

    /**
     * @brief Less-than operator for map/set usage
     */
    bool operator<(const BoardPosition& other) const;

    /**
     * @brief Adds offset to position
     * @param offset Pair of (dx, dy)
     * @return New position with applied offset
     */
    BoardPosition operator+(const std::pair<int,int>& offset) const;
};

