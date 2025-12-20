#include "../../../include/board/surfaces/SphericalSurface.hpp"
#include "../../../include/common/BoardPosition.hpp"
#include <cmath>

SphericalSurface::SphericalSurface(int surfaceId, int radius)
    : Surface(surfaceId, SurfaceType::SPHERICAL), radius(radius) {
}

void SphericalSurface::initializeBoard() {
    // Используем сферические координаты для аппроксимации
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

void SphericalSurface::setupInitialPosition() {
    // TODO: Реализовать расстановку фигур
}

bool SphericalSurface::isValidCoordinate(int x, int y) const {
    return (x * x + y * y) <= (radius * radius);
}

