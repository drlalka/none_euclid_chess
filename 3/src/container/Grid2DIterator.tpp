#ifndef GRID2DITERATOR_TPP
#define GRID2DITERATOR_TPP

template<typename T, bool IsConst>
Grid2DIterator<T, IsConst>::Grid2DIterator()
    : current(nullptr), index(0) {}

template<typename T, bool IsConst>
Grid2DIterator<T, IsConst>::Grid2DIterator(node_pointer node, const std::vector<node_pointer>& nodes, size_t idx)
    : current(node), all_nodes(nodes), index(idx) {}

template<typename T, bool IsConst>
typename Grid2DIterator<T, IsConst>::reference Grid2DIterator<T, IsConst>::operator*() const {
    return current->getData();
}

template<typename T, bool IsConst>
typename Grid2DIterator<T, IsConst>::pointer Grid2DIterator<T, IsConst>::operator->() const {
    return &(current->getData());
}

template<typename T, bool IsConst>
Grid2DIterator<T, IsConst>& Grid2DIterator<T, IsConst>::operator++() {
    if (index < all_nodes.size() - 1) {
        ++index;
        current = all_nodes[index];
    } else {
        current = nullptr;
        index = all_nodes.size();
    }
    return *this;
}

template<typename T, bool IsConst>
Grid2DIterator<T, IsConst> Grid2DIterator<T, IsConst>::operator++(int) {
    Grid2DIterator temp = *this;
    ++(*this);
    return temp;
}

template<typename T, bool IsConst>
Grid2DIterator<T, IsConst>& Grid2DIterator<T, IsConst>::operator--() {
    if (index > 0) {
        --index;
        current = all_nodes[index];
    }
    return *this;
}

template<typename T, bool IsConst>
Grid2DIterator<T, IsConst> Grid2DIterator<T, IsConst>::operator--(int) {
    Grid2DIterator temp = *this;
    --(*this);
    return temp;
}

template<typename T, bool IsConst>
bool Grid2DIterator<T, IsConst>::operator==(const Grid2DIterator& other) const {
    return current == other.current && index == other.index;
}

template<typename T, bool IsConst>
bool Grid2DIterator<T, IsConst>::operator!=(const Grid2DIterator& other) const {
    return !(*this == other);
}

#endif

