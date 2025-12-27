#include "../../include/common/Move.hpp"
#include <chrono>

Move::Move(const BoardPosition& from, const BoardPosition& to, bool portalUsed)
    : from(from), to(to), portalUsed(portalUsed),
      movingPieceColor(PieceColor::WHITE),
      capturedType(PieceType::PAWN),
      hadCapture(false), pieceHadMoved(false) {
    timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

const BoardPosition& Move::getFrom() const {
    return from;
}

const BoardPosition& Move::getTo() const {
    return to;
}

long Move::getTimestamp() const {
    return timestamp;
}

bool Move::isPortalUsed() const {
    return portalUsed;
}

void Move::setMovingPieceColor(PieceColor color) {
    movingPieceColor = color;
}

PieceColor Move::getMovingPieceColor() const {
    return movingPieceColor;
}

void Move::setCapturedType(PieceType type) {
    capturedType = type;
    hadCapture = true;
}

PieceType Move::getCapturedType() const {
    return capturedType;
}

bool Move::hadPieceCapture() const {
    return hadCapture;
}

void Move::setPieceHadMoved(bool moved) {
    pieceHadMoved = moved;
}

bool Move::getPieceHadMoved() const {
    return pieceHadMoved;
}

bool Move::operator==(const Move& other) const {
    return from == other.from && to == other.to;
}

