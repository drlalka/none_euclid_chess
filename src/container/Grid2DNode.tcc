#pragma once

template<typename T>
Grid2DNode<T>::Grid2DNode(const T& value, int x, int y)
    : data(value), x(x), y(y) {
    neighbors.fill(nullptr);
}

template<typename T>
Grid2DNode<T>::Grid2DNode(T&& value, int x, int y)
    : data(std::move(value)), x(x), y(y) {
    neighbors.fill(nullptr);
}

template<typename T>
T& Grid2DNode<T>::getData() {
    return data;
}

template<typename T>
const T& Grid2DNode<T>::getData() const {
    return data;
}

template<typename T>
void Grid2DNode<T>::setData(const T& value) {
    data = value;
}

template<typename T>
void Grid2DNode<T>::setData(T&& value) {
    data = std::move(value);
}

template<typename T>
Grid2DNode<T>* Grid2DNode<T>::getNeighbor(Direction dir) const {
    return neighbors[dir];
}

template<typename T>
void Grid2DNode<T>::setNeighbor(Direction dir, Grid2DNode* node) {
    neighbors[dir] = node;
}

template<typename T>
bool Grid2DNode<T>::hasNeighbor(Direction dir) const {
    return neighbors[dir] != nullptr;
}

template<typename T>
std::vector<Grid2DNode<T>*> Grid2DNode<T>::getAllNeighbors() const {
    std::vector<Grid2DNode*> result;
    for (auto* neighbor : neighbors) {
        if (neighbor != nullptr) {
            result.push_back(neighbor);
        }
    }
    return result;
}
