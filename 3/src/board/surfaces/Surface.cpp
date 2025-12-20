#include "../../../include/board/surfaces/Surface.hpp"
#include "../../../include/pieces/ChessPiece.hpp"

Surface::Surface(int surfaceId, SurfaceType type)
    : surfaceId(surfaceId), surfaceType(type) {
}

Surface::~Surface() {
    for (auto& pair : cells) {
        delete pair.second;
    }
    for (auto& pair : portals) {
        delete pair.second;
    }
}

Cell* Surface::getCell(int x, int y) {
    auto key = std::make_pair(x, y);
    auto it = cells.find(key);
    if (it != cells.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<Cell*> Surface::getNeighborCells(int x, int y) const {
    std::vector<Cell*> neighbors;
    // Будет реализовано в наследниках
    return neighbors;
}

ChessPiece* Surface::getPieceAt(int x, int y) const {
    auto key = std::make_pair(x, y);
    auto it = cells.find(key);
    if (it != cells.end()) {
        return it->second->getPiece();
    }
    return nullptr;
}

void Surface::addPortal(int fromX, int fromY, int toSurfaceId, int toX, int toY) {
    auto key = std::make_pair(fromX, fromY);
    PortalLink* portal = new PortalLink(toSurfaceId, toX, toY);
    portals[key] = portal;

    Cell* cell = getCell(fromX, fromY);
    if (cell != nullptr) {
        cell->setPortalLink(portal);
    }
}

void Surface::deletePortal(int x, int y) {
    auto key = std::make_pair(x, y);
    auto it = portals.find(key);
    if (it != portals.end()) {
        delete it->second;
        portals.erase(it);

        Cell* cell = getCell(x, y);
        if (cell != nullptr) {
            cell->setPortalLink(nullptr);
        }
    }
}

void Surface::addPiece(ChessPiece* piece, int x, int y) {
    Cell* cell = getCell(x, y);
    if (cell != nullptr) {
        cell->setPiece(piece);
    }
}

ChessPiece* Surface::removePiece(int x, int y) {
    Cell* cell = getCell(x, y);
    if (cell != nullptr) {
        return cell->removePiece();
    }
    return nullptr;
}

int Surface::getSurfaceId() const {
    return surfaceId;
}

