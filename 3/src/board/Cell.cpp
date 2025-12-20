#include "../../include/board/Cell.hpp"
#include "../../include/board/PortalLink.hpp"
#include "../../include/pieces/ChessPiece.hpp"

Cell::Cell(const BoardPosition& position)
    : position(position), piece(nullptr), portalLink(nullptr) {
}

Cell::~Cell() {
    // Pieces and portals are managed externally
}

bool Cell::isEmpty() const {
    return piece == nullptr;
}

bool Cell::hasPortal() const {
    return portalLink != nullptr;
}

ChessPiece* Cell::getPiece() const {
    return piece;
}

PortalLink* Cell::getPortalDestination() const {
    return portalLink;
}

void Cell::setPiece(ChessPiece* piece) {
    this->piece = piece;
}

ChessPiece* Cell::removePiece() {
    ChessPiece* temp = piece;
    piece = nullptr;
    return temp;
}

const BoardPosition& Cell::getPosition() const {
    return position;
}

void Cell::setPortalLink(PortalLink* link) {
    portalLink = link;
}

