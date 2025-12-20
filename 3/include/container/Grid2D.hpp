
#ifndef GRID2D_HPP
#define GRID2D_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <map>
#include <vector>
#include <algorithm>
#include "Grid2DNode.hpp"
#include "Grid2DIterator.hpp"

/**
 * @brief Template container for storing elements in 2D graph with 8-directional links
 * @tparam T Type of stored elements
 */
template<typename T>
class Grid2D {
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using iterator = Grid2DIterator<T, false>;
    using const_iterator = Grid2DIterator<T, true>;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

private:
    std::map<std::pair<int, int>, Grid2DNode<T>*> nodes;
    size_type element_count;

    void linkNeighbors(Grid2DNode<T>* node, int x, int y);
    void unlinkNode(Grid2DNode<T>* node);
    Grid2DNode<T>* findNode(int x, int y) const;
    void deepCopy(const Grid2D& other);

public:
    /**
     * @brief Default constructor
     */
    Grid2D();

    /**
     * @brief Copy constructor
     * @param other Container to copy from
     */
    Grid2D(const Grid2D& other);

    /**
     * @brief Move constructor
     * @param other Container to move from
     */
    Grid2D(Grid2D&& other) noexcept;

    /**
     * @brief Destructor
     */
    ~Grid2D();

    /**
     * @brief Copy assignment operator
     */
    Grid2D& operator=(const Grid2D& other);

    /**
     * @brief Move assignment operator
     */
    Grid2D& operator=(Grid2D&& other) noexcept;

    /**
     * @brief Access element with bounds checking
     * @param x X coordinate
     * @param y Y coordinate
     * @return Reference to element
     * @throws std::out_of_range if element does not exist
     */
    reference at(int x, int y);
    const_reference at(int x, int y) const;

    /**
     * @brief Access element using () operator
     * @param x X coordinate
     * @param y Y coordinate
     * @return Reference to element
     */
    reference operator()(int x, int y);
    const_reference operator()(int x, int y) const;

    /**
     * @brief Insert element by copy
     * @param x X coordinate
     * @param y Y coordinate
     * @param value Element value
     */
    void insert(int x, int y, const T& value);

    /**
     * @brief Insert element by move
     * @param x X coordinate
     * @param y Y coordinate
     * @param value Element value
     */
    void insert(int x, int y, T&& value);

    /**
     * @brief Remove element
     * @param x X coordinate
     * @param y Y coordinate
     */
    void erase(int x, int y);

    /**
     * @brief Check if element exists
     * @param x X coordinate
     * @param y Y coordinate
     * @return true if element exists
     */
    bool contains(int x, int y) const;

    /**
     * @brief Check if container is empty
     * @return true if container is empty
     */
    bool empty() const;

    /**
     * @brief Get number of elements
     * @return Number of elements
     */
    size_type size() const;

    /**
     * @brief Clear container
     */
    void clear();

    /**
     * @brief Swap contents with another container
     * @param other Another container
     */
    void swap(Grid2D& other) noexcept;

    /**
     * @brief Get iterator to beginning
     */
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    /**
     * @brief Get node by coordinates
     * @param x X coordinate
     * @param y Y coordinate
     * @return Pointer to node or nullptr
     */
    Grid2DNode<T>* getNode(int x, int y) const;

    /**
     * @brief Get all neighbor nodes
     * @param x X coordinate
     * @param y Y coordinate
     * @return Vector of pointers to neighbor nodes
     */
    std::vector<Grid2DNode<T>*> getNeighborNodes(int x, int y) const;
};

#include "../../src/container/Grid2D.tpp"

#endif
