#include "../../include/pieces/ChessPiece.hpp"
#include "../../include/board/GameBoard.hpp"

ChessPiece::ChessPiece(PieceColor color, PieceType type, const BoardPosition& position)
    : color(color), type(type), position(position) {
}

PieceType ChessPiece::getType() const {
    return type;
}

int ChessPiece::getValue() const {
    switch (type) {
        case PieceType::PAWN:   return 1;
        case PieceType::KNIGHT: return 3;
        case PieceType::BISHOP: return 3;
        case PieceType::ROOK:   return 5;
        case PieceType::QUEEN:  return 9;
        case PieceType::KING:   return 0; // Король бесценен
        default: return 0;
    }
}

PieceColor ChessPiece::getColor() const {
    return color;
}

const BoardPosition& ChessPiece::getPosition() const {
    return position;
}

void ChessPiece::moveTo(const BoardPosition& newPosition) {
    position = newPosition;
}

