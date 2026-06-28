#include "../../../include/board/surfaces/CylindricalSurface.hpp"
#include "../../../include/common/BoardPosition.hpp"
#include "../../../include/pieces/Pawn.hpp"
#include "../../../include/pieces/Rook.hpp"
#include "../../../include/pieces/Knight.hpp"
#include "../../../include/pieces/Bishop.hpp"
#include "../../../include/pieces/Queen.hpp"
#include "../../../include/pieces/King.hpp"

CylindricalSurface::CylindricalSurface(int surfaceId, int height)
    : Surface(surfaceId, SurfaceType::CYLINDRICAL), height(height) {
}

void CylindricalSurface::initializeBoard() {
    for (int x = 0; x < CIRCUMFERENCE; ++x) {
        for (int y = 0; y < height; ++y) {
            BoardPosition pos(surfaceId, x, y);
            Cell* cell = new Cell(pos);
            cells[std::make_pair(x, y)] = cell;
        }
    }
    for (int x = 0; x < CIRCUMFERENCE; ++x) {
        for (int y = 0; y < height; ++y) {
            Cell* cell = cells[std::make_pair(x, y)];
            int nextX = (x + 1) % CIRCUMFERENCE;
            int prevX = (x - 1 + CIRCUMFERENCE) % CIRCUMFERENCE;

            cell->setNeighbor(Cell::Direction::EAST, cells[std::make_pair(nextX, y)]);
            cell->setNeighbor(Cell::Direction::WEST, cells[std::make_pair(prevX, y)]);
            if (y > 0) {
                cell->setNeighbor(Cell::Direction::SOUTH, cells[std::make_pair(x, y - 1)]);
            }
            if (y < height - 1) {
                cell->setNeighbor(Cell::Direction::NORTH, cells[std::make_pair(x, y + 1)]);
            }

            if (y > 0) {
                cell->setNeighbor(Cell::Direction::SOUTHWEST, cells[std::make_pair(prevX, y - 1)]);
                cell->setNeighbor(Cell::Direction::SOUTHEAST, cells[std::make_pair(nextX, y - 1)]);
            }
            if (y < height - 1) {
                cell->setNeighbor(Cell::Direction::NORTHWEST, cells[std::make_pair(prevX, y + 1)]);
                cell->setNeighbor(Cell::Direction::NORTHEAST, cells[std::make_pair(nextX, y + 1)]);
            }
        }
    }
}

void CylindricalSurface::setupInitialPosition() {
    for (int x = 0; x < CIRCUMFERENCE; ++x) {
        getCell(x, 1)->setPiece(
            std::make_unique<Pawn>(PieceColor::WHITE, BoardPosition(surfaceId, x, 1))
        );

        getCell(x, height - 2)->setPiece(
            std::make_unique<Pawn>(PieceColor::BLACK, BoardPosition(surfaceId, x, height - 2))
        );

        switch (x) {
            case 0:
            case 7:
                getCell(x, 0)->setPiece(
                    std::make_unique<Rook>(PieceColor::WHITE, BoardPosition(surfaceId, x, 0))
                );
                getCell(x, height - 1)->setPiece(
                    std::make_unique<Rook>(PieceColor::BLACK, BoardPosition(surfaceId, x, height - 1))
                );
                break;

            case 1:
            case 6:
                getCell(x, 0)->setPiece(
                    std::make_unique<Knight>(PieceColor::WHITE, BoardPosition(surfaceId, x, 0))
                );
                getCell(x, height - 1)->setPiece(
                    std::make_unique<Knight>(PieceColor::BLACK, BoardPosition(surfaceId, x, height - 1))
                );
                break;

            case 2:
            case 5:
                getCell(x, 0)->setPiece(
                    std::make_unique<Bishop>(PieceColor::WHITE, BoardPosition(surfaceId, x, 0))
                );
                getCell(x, height - 1)->setPiece(
                    std::make_unique<Bishop>(PieceColor::BLACK, BoardPosition(surfaceId, x, height - 1))
                );
                break;

            case 3:
                getCell(x, 0)->setPiece(
                    std::make_unique<Queen>(PieceColor::WHITE, BoardPosition(surfaceId, x, 0))
                );
                getCell(x, height - 1)->setPiece(
                    std::make_unique<Queen>(PieceColor::BLACK, BoardPosition(surfaceId, x, height - 1))
                );
                break;

            case 4:
                getCell(x, 0)->setPiece(
                    std::make_unique<King>(PieceColor::WHITE, BoardPosition(surfaceId, x, 0))
                );
                getCell(x, height - 1)->setPiece(
                    std::make_unique<King>(PieceColor::BLACK, BoardPosition(surfaceId, x, height - 1))
                );
                break;
        }
    }
}

bool CylindricalSurface::isValidCoordinate(int x, int y) const {
    return y >= 0 && y < height;
}



