#include "../../include/board/Cell.hpp"
#include "../../include/pieces/ChessPiece.hpp"

Cell::Cell(const BoardPosition& pos)
    : position(pos), piece(nullptr), portalLink(nullptr) {
    neighbors.fill(nullptr);
}

void Cell::setPiece(std::unique_ptr<ChessPiece> p) {
    piece = std::move(p);
    if (piece) {
        piece->moveTo(position);
    }
}

std::unique_ptr<ChessPiece> Cell::removePiece() {
    return std::move(piece);
}

void Cell::setPortalLink(std::unique_ptr<PortalLink> link) {
    portalLink = std::move(link);
}

BoardPosition Cell::getPortalDestination() const {
    if (portalLink) {
        return portalLink->getDestination();
    }
    return position;
}

void Cell::setNeighbor(Direction dir, Cell* cell) {
    neighbors[dir] = cell;
}

Cell* Cell::getNeighbor(Direction dir) const {
    return neighbors[dir];
}

