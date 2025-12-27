#pragma once

#include <array>
#include <vector>

/**
 * @brief Node of 2D graph with 8-directional links
 * @tparam T Type of stored data
 */
template<typename T>
class Grid2DNode {
public:
    /**
     * @brief Directions for links to neighbor nodes
     */
    enum Direction {
        RIGHT = 0,
        RIGHT_UP = 1,
        UP = 2,
        LEFT_UP = 3,
        LEFT = 4,
        LEFT_DOWN = 5,
        DOWN = 6,
        RIGHT_DOWN = 7
    };

private:
    T data;
    std::array<Grid2DNode*, 8> neighbors;
    int x;
    int y;

public:
    /**
     * @brief Node constructor with value copy
     * @param value Value to store
     * @param x X coordinate
     * @param y Y coordinate
     */
    Grid2DNode(const T& value, int x, int y);

    /**
     * @brief Node constructor with value move
     * @param value Value to store
     * @param x X coordinate
     * @param y Y coordinate
     */
    Grid2DNode(T&& value, int x, int y);

    ~Grid2DNode() = default;

    /**
     * @brief Get reference to data
     * @return Reference to stored data
     */
    T& getData();

    /**
     * @brief Get const reference to data
     * @return Const reference to stored data
     */
    const T& getData() const;

    /**
     * @brief Set data by copy
     * @param value New value
     */
    void setData(const T& value);

    /**
     * @brief Set data by move
     * @param value New value
     */
    void setData(T&& value);

    /**
     * @brief Get neighbor node in direction
     * @param dir Direction
     * @return Pointer to neighbor node or nullptr
     */
    Grid2DNode* getNeighbor(Direction dir) const;

    /**
     * @brief Set neighbor node
     * @param dir Direction
     * @param node Pointer to neighbor node
     */
    void setNeighbor(Direction dir, Grid2DNode* node);

    /**
     * @brief Check if neighbor exists in direction
     * @param dir Direction
     * @return true if neighbor exists
     */
    bool hasNeighbor(Direction dir) const;

    /**
     * @brief Get X coordinate
     * @return X coordinate of node
     */
    int getX() const { return x; }

    /**
     * @brief Get Y coordinate
     * @return Y coordinate of node
     */
    int getY() const { return y; }

    /**
     * @brief Get all existing neighbor nodes
     * @return Vector of pointers to neighbor nodes
     */
    std::vector<Grid2DNode*> getAllNeighbors() const;
};

#include "../../src/container/Grid2DNode.tpp"


