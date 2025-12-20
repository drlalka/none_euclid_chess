#ifndef CELL_HPP
#define CELL_HPP

#include <vector>
#include "../common/BoardPosition.hpp"

class ChessPiece;
class PortalLink;

class Cell {
private:
    BoardPosition position;
    ChessPiece* piece;
    PortalLink* portalLink;
    std::vector<Cell*> neighbors;

public:
    Cell(const BoardPosition& position);
    ~Cell();

    bool isEmpty() const;
    bool hasPortal() const;
    ChessPiece* getPiece() const;
    PortalLink* getPortalDestination() const;
    void setPiece(ChessPiece* piece);
    ChessPiece* removePiece();
    const BoardPosition& getPosition() const;
    void setPortalLink(PortalLink* link);
};

#endif // CELL_HPP

