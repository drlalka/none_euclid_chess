#ifndef SURFACE_HPP
#define SURFACE_HPP

#include <map>
#include <vector>
#include <utility>
#include "../../common/Enums.hpp"
#include "../Cell.hpp"
#include "../PortalLink.hpp"

class ChessPiece;

class Surface {
protected:
    int surfaceId;
    SurfaceType surfaceType;
    std::map<std::pair<int, int>, Cell*> cells;
    std::map<std::pair<int, int>, PortalLink*> portals;

public:
    Surface(int surfaceId, SurfaceType type);
    virtual ~Surface();

    virtual void initializeBoard() = 0;
    virtual void setupInitialPosition() = 0;
    virtual bool isValidCoordinate(int x, int y) const = 0;

    Cell* getCell(int x, int y);
    std::vector<Cell*> getNeighborCells(int x, int y) const;
    ChessPiece* getPieceAt(int x, int y) const;
    void addPortal(int fromX, int fromY, int toSurfaceId, int toX, int toY);
    void deletePortal(int x, int y);
    void addPiece(ChessPiece* piece, int x, int y);
    ChessPiece* removePiece(int x, int y);
    int getSurfaceId() const;
};

#endif // SURFACE_HPP

