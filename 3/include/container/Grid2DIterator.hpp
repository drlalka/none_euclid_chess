#ifndef GRID2DITERATOR_HPP
#define GRID2DITERATOR_HPP

#include <iterator>
#include <vector>
#include "Grid2DNode.hpp"

/**
 * @brief Bidirectional iterator for Grid2D container
 * @tparam T Type of stored elements
 * @tparam IsConst true for const_iterator, false for iterator
 */
template<typename T, bool IsConst>
class Grid2DIterator {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = typename std::conditional<IsConst, const T*, T*>::type;
    using reference = typename std::conditional<IsConst, const T&, T&>::type;
    using node_pointer = typename std::conditional<IsConst, const Grid2DNode<T>*, Grid2DNode<T>*>::type;

private:
    node_pointer current;
    std::vector<node_pointer> all_nodes;
    size_t index;

public:
    /**
     * @brief Default constructor
     */
    Grid2DIterator();

    /**
     * @brief Iterator constructor
     * @param node Current node
     * @param nodes All container nodes
     * @param idx Index of current node
     */
    Grid2DIterator(node_pointer node, const std::vector<node_pointer>& nodes, size_t idx);

    /**
     * @brief Dereference iterator
     * @return Reference to element
     */
    reference operator*() const;

    /**
     * @brief Access element members
     * @return Pointer to element
     */
    pointer operator->() const;

    /**
     * @brief Prefix increment
     */
    Grid2DIterator& operator++();

    /**
     * @brief Postfix increment
     */
    Grid2DIterator operator++(int);

    /**
     * @brief Prefix decrement
     */
    Grid2DIterator& operator--();

    /**
     * @brief Postfix decrement
     */
    Grid2DIterator operator--(int);

    /**
     * @brief Compare iterators for equality
     */
    bool operator==(const Grid2DIterator& other) const;

    /**
     * @brief Compare iterators for inequality
     */
    bool operator!=(const Grid2DIterator& other) const;

    /**
     * @brief Get pointer to node
     */
    node_pointer getNode() const { return current; }

    /**
     * @brief Get X coordinate of current node
     */
    int getX() const { return current ? current->getX() : -1; }

    /**
     * @brief Get Y coordinate of current node
     */
    int getY() const { return current ? current->getY() : -1; }
};

#include "../../src/container/Grid2DIterator.tpp"

#endif

