#include "../../../include/board/surfaces/Surface.hpp"
#include "../../../include/pieces/ChessPiece.hpp"

Surface::Surface(int surfaceId, SurfaceType type)
    : surfaceId(surfaceId), surfaceType(type) {
}

Surface::~Surface() {
    for (auto& pair : cells) {
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
    BoardPosition target(toSurfaceId, toX, toY);
    auto portal = std::make_unique<PortalLink>(target);

    Cell* cell = getCell(fromX, fromY);
    if (cell != nullptr) {
        cell->setPortalLink(std::move(portal));
    }

    portals[key] = cell->getPortalLink();
}

void Surface::deletePortal(int x, int y) {
    auto key = std::make_pair(x, y);
    portals.erase(key);

    Cell* cell = getCell(x, y);
    if (cell != nullptr) {
        cell->setPortalLink(nullptr);
    }
}

void Surface::addPiece(std::unique_ptr<ChessPiece> piece, int x, int y) {
    Cell* cell = getCell(x, y);
    if (cell != nullptr) {
        cell->setPiece(std::move(piece));
    }
}

std::unique_ptr<ChessPiece> Surface::removePiece(int x, int y) {
    Cell* cell = getCell(x, y);
    if (cell != nullptr) {
        return cell->removePiece();
    }
    return nullptr;
}

std::vector<ChessPiece*> Surface::getAllPieces() const {
    std::vector<ChessPiece*> pieces;
    for (const auto& pair : cells) {
        ChessPiece* piece = pair.second->getPiece();
        if (piece != nullptr) {
            pieces.push_back(piece);
        }
    }
    return pieces;
}

int Surface::getSurfaceId() const {
    return surfaceId;
}

