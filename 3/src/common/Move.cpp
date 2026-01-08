#include "../../include/common/Move.hpp"
#include <chrono>

Move::Move(const BoardPosition& from, const BoardPosition& to, bool portalUsed)
    : from(from), to(to), portalUsed(portalUsed),
      movingPieceColor(PieceColor::WHITE),
      capturedType(PieceType::PAWN),
      hadCapture(false), pieceHadMoved(false) {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    timestamp = ms.count();
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

void Move::setPortalUsed(bool used) {
    portalUsed = used;
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

