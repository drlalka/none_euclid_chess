#include "../../include/board/GameBoard.hpp"
#include "../../include/pieces/ChessPiece.hpp"
#include "../../include/pieces/Pawn.hpp"
#include "../../include/pieces/Rook.hpp"
#include "../../include/pieces/King.hpp"
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

    bool hadMoved = false;
    if (piece->getType() == PieceType::PAWN) {
        hadMoved = static_cast<Pawn*>(piece)->getHasMoved();
    } else if (piece->getType() == PieceType::ROOK) {
        hadMoved = static_cast<Rook*>(piece)->getHasMoved();
    } else if (piece->getType() == PieceType::KING) {
        hadMoved = static_cast<King*>(piece)->getHasMoved();
    }
    move.setPieceHadMoved(hadMoved);

    auto capturedPiece = toSurface->removePiece(to.getX(), to.getY());

    if (capturedPiece != nullptr) {
        move.setCapturedType(capturedPiece->getType());
    }

    auto movedPiece = fromSurface->removePiece(from.getX(), from.getY());
    movedPiece->moveTo(to);

    if (movedPiece->getType() == PieceType::PAWN) {
        static_cast<Pawn*>(movedPiece.get())->setMoved(true);
    } else if (movedPiece->getType() == PieceType::ROOK) {
        static_cast<Rook*>(movedPiece.get())->setMoved(true);
    } else if (movedPiece->getType() == PieceType::KING) {
        static_cast<King*>(movedPiece.get())->setMoved(true);
    }

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
