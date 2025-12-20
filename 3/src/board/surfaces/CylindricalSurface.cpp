#include "../../../include/board/surfaces/CylindricalSurface.hpp"
#include "../../../include/common/BoardPosition.hpp"

CylindricalSurface::CylindricalSurface(int surfaceId, int circumference, int height)
    : Surface(surfaceId, SurfaceType::CYLINDRICAL), circumference(circumference), height(height) {
}

void CylindricalSurface::initializeBoard() {
    for (int x = 0; x < circumference; ++x) {
        for (int y = 0; y < height; ++y) {
            BoardPosition pos(surfaceId, x, y);
            Cell* cell = new Cell(pos);
            cells[std::make_pair(x, y)] = cell;
        }
    }
}

void CylindricalSurface::setupInitialPosition() {
    // TODO: Реализовать расстановку фигур
}

bool CylindricalSurface::isValidCoordinate(int x, int y) const {
    // X wraps around (cylindrical), Y is bounded
    return y >= 0 && y < height;
}

