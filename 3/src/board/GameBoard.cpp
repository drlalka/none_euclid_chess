#include "../../include/board/GameBoard.hpp"
#include "../../include/pieces/ChessPiece.hpp"
#include "../../include/board/surfaces/Surface.hpp"

GameBoard::GameBoard() {
}

GameBoard::~GameBoard() {
    for (Surface* surface : surfaces) {
        delete surface;
    }
}

Surface* GameBoard::getSurface(int id) const {
    auto it = surfaceMap.find(id);
    if (it != surfaceMap.end()) {
        return it->second;
    }
    return nullptr;
}

ChessPiece* GameBoard::movePiece(const BoardPosition& from, const BoardPosition& to) {
    Surface* fromSurface = getSurface(from.getSurfaceId());
    Surface* toSurface = getSurface(to.getSurfaceId());

    if (fromSurface == nullptr || toSurface == nullptr) {
        return nullptr;
    }

    ChessPiece* piece = fromSurface->removePiece(from.getX(), from.getY());
    ChessPiece* captured = toSurface->removePiece(to.getX(), to.getY());

    if (piece != nullptr) {
        piece->moveTo(to);
        toSurface->addPiece(piece, to.getX(), to.getY());
    }

    return captured;
}

bool GameBoard::isValidMove(const BoardPosition& from, const BoardPosition& to) const {
    // TODO: Реализовать проверку валидности хода
    return true;
}

ChessPiece* GameBoard::getPieceAt(const BoardPosition& pos) const {
    Surface* surface = getSurface(pos.getSurfaceId());
    if (surface != nullptr) {
        return surface->getPieceAt(pos.getX(), pos.getY());
    }
    return nullptr;
}

void GameBoard::addSurface(Surface* surface) {
    surfaces.push_back(surface);
    surfaceMap[surface->getSurfaceId()] = surface;
}

std::vector<ChessPiece*> GameBoard::getAllPieces() const {
    std::vector<ChessPiece*> allPieces;

    for (Surface* surface : surfaces) {
        // TODO: Собрать все фигуры с поверхности
    }

    return allPieces;
}

