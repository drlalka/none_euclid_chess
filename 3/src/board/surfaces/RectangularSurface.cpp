#include "../../../include/board/surfaces/RectangularSurface.hpp"
#include "../../../include/common/BoardPosition.hpp"

RectangularSurface::RectangularSurface(int surfaceId, int width, int height)
    : Surface(surfaceId, SurfaceType::RECTANGULAR), width(width), height(height) {
}

void RectangularSurface::initializeBoard() {
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            BoardPosition pos(surfaceId, x, y);
            Cell* cell = new Cell(pos);
            cells[std::make_pair(x, y)] = cell;
        }
    }
}

void RectangularSurface::setupInitialPosition() {
    // TODO: Реализовать расстановку фигур
}

bool RectangularSurface::isValidCoordinate(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

