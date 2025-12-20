#include "../../../include/board/surfaces/CircularSurface.hpp"
#include "../../../include/common/BoardPosition.hpp"
#include <cmath>

CircularSurface::CircularSurface(int surfaceId, int radius)
    : Surface(surfaceId, SurfaceType::CIRCULAR), radius(radius) {
}

void CircularSurface::initializeBoard() {
    for (int x = -radius; x <= radius; ++x) {
        for (int y = -radius; y <= radius; ++y) {
            if (x * x + y * y <= radius * radius) {
                BoardPosition pos(surfaceId, x, y);
                Cell* cell = new Cell(pos);
                cells[std::make_pair(x, y)] = cell;
            }
        }
    }
}

void CircularSurface::setupInitialPosition() {
    // TODO: Реализовать расстановку фигур
}

bool CircularSurface::isValidCoordinate(int x, int y) const {
    return (x * x + y * y) <= (radius * radius);
}

