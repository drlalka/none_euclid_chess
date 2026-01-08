#include "../../include/board/GameBoard.hpp"
#include "../../include/pieces/ChessPiece.hpp"
#include "../../include/board/surfaces/Surface.hpp"

GameBoard::GameBoard() {
}

Surface* GameBoard::getSurface(int id) const {
    for (const auto& surface : surfaces) {
        if (surface->getSurfaceId() == id) {
            return surface.get();
        }
    }
    return nullptr;
}

std::unique_ptr<ChessPiece> GameBoard::movePiece(Move& move) {
    const BoardPosition& from = move.getFrom();
    const BoardPosition& to = move.getTo();

    Surface* fromSurface = getSurface(from.getSurfaceId());
    Surface* toSurface = getSurface(to.getSurfaceId());

    if (fromSurface == nullptr || toSurface == nullptr) {
        return nullptr;
    }

    ChessPiece* piece = fromSurface->getPieceAt(from.getX(), from.getY());
    if (piece == nullptr) {
        return nullptr;
    }

    move.setMovingPieceColor(piece->getColor());

    piece->beforeMove(move);

    auto capturedPiece = toSurface->removePiece(to.getX(), to.getY());

    if (capturedPiece != nullptr) {
        move.setCapturedType(capturedPiece->getType());
    }

    auto movedPiece = fromSurface->removePiece(from.getX(), from.getY());
    movedPiece->moveTo(to);

    movedPiece->onMove(move);

    toSurface->addPiece(std::move(movedPiece), to.getX(), to.getY());

    return capturedPiece;
}

bool GameBoard::isValidMove(const BoardPosition& from, const BoardPosition& to) const {
    return true;
}

ChessPiece* GameBoard::getPieceAt(const BoardPosition& pos) const {
    Surface* surface = getSurface(pos.getSurfaceId());
    if (surface != nullptr) {
        return surface->getPieceAt(pos.getX(), pos.getY());
    }
    return nullptr;
}

void GameBoard::addSurface(std::unique_ptr<Surface> surface) {
    surfaces.push_back(std::move(surface));
}

std::vector<ChessPiece*> GameBoard::getAllPieces() const {
    std::vector<ChessPiece*> allPieces;

    for (const auto& surface : surfaces) {
        std::vector<ChessPiece*> surfacePieces = surface->getAllPieces();
        allPieces.insert(allPieces.end(), surfacePieces.begin(), surfacePieces.end());
    }

    return allPieces;
}
