#ifndef GRID2D_TPP
#define GRID2D_TPP

template<typename T>
Grid2D<T>::Grid2D() : element_count(0) {}

template<typename T>
Grid2D<T>::Grid2D(const Grid2D& other) : element_count(0) {
    deepCopy(other);
}

template<typename T>
Grid2D<T>::Grid2D(Grid2D&& other) noexcept
    : nodes(std::move(other.nodes)), element_count(other.element_count) {
    other.element_count = 0;
}

template<typename T>
Grid2D<T>::~Grid2D() {
    clear();
}

template<typename T>
Grid2D<T>& Grid2D<T>::operator=(const Grid2D& other) {
    if (this != &other) {
        clear();
        deepCopy(other);
    }
    return *this;
}

template<typename T>
Grid2D<T>& Grid2D<T>::operator=(Grid2D&& other) noexcept {
    if (this != &other) {
        clear();
        nodes = std::move(other.nodes);
        element_count = other.element_count;
        other.element_count = 0;
    }
    return *this;
}

template<typename T>
typename Grid2D<T>::reference Grid2D<T>::at(int x, int y) {
    Grid2DNode<T>* node = findNode(x, y);
    if (!node) {
        throw std::out_of_range("Grid2D::at: coordinates out of range");
    }
    return node->getData();
}

template<typename T>
typename Grid2D<T>::const_reference Grid2D<T>::at(int x, int y) const {
    Grid2DNode<T>* node = findNode(x, y);
    if (!node) {
        throw std::out_of_range("Grid2D::at: coordinates out of range");
    }
    return node->getData();
}

template<typename T>
typename Grid2D<T>::reference Grid2D<T>::operator()(int x, int y) {
    return at(x, y);
}

template<typename T>
typename Grid2D<T>::const_reference Grid2D<T>::operator()(int x, int y) const {
    return at(x, y);
}

template<typename T>
void Grid2D<T>::insert(int x, int y, const T& value) {
    auto key = std::make_pair(x, y);
    if (nodes.find(key) != nodes.end()) {
        nodes[key]->setData(value);
        return;
    }

    auto* node = new Grid2DNode<T>(value, x, y);
    nodes[key] = node;
    linkNeighbors(node, x, y);
    ++element_count;
}

template<typename T>
void Grid2D<T>::insert(int x, int y, T&& value) {
    auto key = std::make_pair(x, y);
    if (nodes.find(key) != nodes.end()) {
        nodes[key]->setData(std::move(value));
        return;
    }

    auto* node = new Grid2DNode<T>(std::move(value), x, y);
    nodes[key] = node;
    linkNeighbors(node, x, y);
    ++element_count;
}

template<typename T>
void Grid2D<T>::erase(int x, int y) {
    auto key = std::make_pair(x, y);
    auto it = nodes.find(key);
    if (it != nodes.end()) {
        unlinkNode(it->second);
        delete it->second;
        nodes.erase(it);
        --element_count;
    }
}

template<typename T>
bool Grid2D<T>::contains(int x, int y) const {
    return nodes.find(std::make_pair(x, y)) != nodes.end();
}

template<typename T>
bool Grid2D<T>::empty() const {
    return element_count == 0;
}

template<typename T>
typename Grid2D<T>::size_type Grid2D<T>::size() const {
    return element_count;
}

template<typename T>
void Grid2D<T>::clear() {
    for (auto& pair : nodes) {
        delete pair.second;
    }
    nodes.clear();
    element_count = 0;
}

template<typename T>
void Grid2D<T>::swap(Grid2D& other) noexcept {
    std::swap(nodes, other.nodes);
    std::swap(element_count, other.element_count);
}

template<typename T>
typename Grid2D<T>::iterator Grid2D<T>::begin() {
    std::vector<Grid2DNode<T>*> node_list;
    for (auto& pair : nodes) {
        node_list.push_back(pair.second);
    }
    if (node_list.empty()) {
        return iterator(nullptr, node_list, 0);
    }
    return iterator(node_list[0], node_list, 0);
}

template<typename T>
typename Grid2D<T>::iterator Grid2D<T>::end() {
    std::vector<Grid2DNode<T>*> node_list;
    for (auto& pair : nodes) {
        node_list.push_back(pair.second);
    }
    return iterator(nullptr, node_list, node_list.size());
}

template<typename T>
typename Grid2D<T>::const_iterator Grid2D<T>::begin() const {
    std::vector<const Grid2DNode<T>*> node_list;
    for (const auto& pair : nodes) {
        node_list.push_back(pair.second);
    }
    if (node_list.empty()) {
        return const_iterator(nullptr, node_list, 0);
    }
    return const_iterator(node_list[0], node_list, 0);
}

template<typename T>
typename Grid2D<T>::const_iterator Grid2D<T>::end() const {
    std::vector<const Grid2DNode<T>*> node_list;
    for (const auto& pair : nodes) {
        node_list.push_back(pair.second);
    }
    return const_iterator(nullptr, node_list, node_list.size());
}

template<typename T>
typename Grid2D<T>::const_iterator Grid2D<T>::cbegin() const {
    return begin();
}

template<typename T>
typename Grid2D<T>::const_iterator Grid2D<T>::cend() const {
    return end();
}

template<typename T>
Grid2DNode<T>* Grid2D<T>::getNode(int x, int y) const {
    return findNode(x, y);
}

template<typename T>
std::vector<Grid2DNode<T>*> Grid2D<T>::getNeighborNodes(int x, int y) const {
    Grid2DNode<T>* node = findNode(x, y);
    if (!node) {
        return {};
    }
    return node->getAllNeighbors();
}

template<typename T>
void Grid2D<T>::linkNeighbors(Grid2DNode<T>* node, int x, int y) {
    using Dir = typename Grid2DNode<T>::Direction;

    std::pair<int, int> neighborOffsets[8] = {
        {1, 0},
        {1, 1},
        {0, 1},
        {-1, 1},
        {-1, 0},
        {-1, -1},
        {0, -1},
        {1, -1}
    };

    for (int i = 0; i < 8; ++i) {
        int nx = x + neighborOffsets[i].first;
        int ny = y + neighborOffsets[i].second;

        auto neighborKey = std::make_pair(nx, ny);
        auto it = nodes.find(neighborKey);
        if (it != nodes.end()) {
            Grid2DNode<T>* neighbor = it->second;
            node->setNeighbor(static_cast<Dir>(i), neighbor);
            neighbor->setNeighbor(static_cast<Dir>((i + 4) % 8), node);
        }
    }
}

template<typename T>
void Grid2D<T>::unlinkNode(Grid2DNode<T>* node) {
    using Dir = typename Grid2DNode<T>::Direction;

    for (int i = 0; i < 8; ++i) {
        Grid2DNode<T>* neighbor = node->getNeighbor(static_cast<Dir>(i));
        if (neighbor) {
            neighbor->setNeighbor(static_cast<Dir>((i + 4) % 8), nullptr);
        }
    }
}

template<typename T>
Grid2DNode<T>* Grid2D<T>::findNode(int x, int y) const {
    auto it = nodes.find(std::make_pair(x, y));
    if (it != nodes.end()) {
        return it->second;
    }
    return nullptr;
}

template<typename T>
void Grid2D<T>::deepCopy(const Grid2D& other) {
    for (const auto& pair : other.nodes) {
        int x = pair.first.first;
        int y = pair.first.second;
        insert(x, y, pair.second->getData());
    }
}


#endif

