#include "../../include/common/BoardPosition.hpp"

BoardPosition::BoardPosition(int surfaceId, int x, int y)
    : surfaceId(surfaceId), x(x), y(y) {
}

int BoardPosition::getSurfaceId() const {
    return surfaceId;
}

int BoardPosition::getX() const {
    return x;
}

int BoardPosition::getY() const {
    return y;
}

bool BoardPosition::equals(const BoardPosition& other) const {
    return surfaceId == other.surfaceId && x == other.x && y == other.y;
}

