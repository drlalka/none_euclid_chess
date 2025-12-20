#include "../../include/pieces/Pawn.hpp"
#include "../../include/pieces/Queen.hpp"
#include "../../include/pieces/Rook.hpp"
#include "../../include/pieces/Bishop.hpp"
#include "../../include/pieces/Knight.hpp"
#include "../../include/board/GameBoard.hpp"

Pawn::Pawn(PieceColor color, const BoardPosition& position)
    : ChessPiece(color, PieceType::PAWN, position), hasMoved(false) {
}

std::vector<Move> Pawn::getPossibleMoves(GameBoard* board) const {
    std::vector<Move> moves;
    // TODO: Реализовать получение возможных ходов пешки
    return moves;
}

bool Pawn::isValidMove(const Move& move, GameBoard* board) const {
    // TODO: Реализовать проверку валидности хода пешки
    return false;
}

int Pawn::getMaxPortalUse() const {
    return 1; // Пешка может использовать 1 портал за ход
}

bool Pawn::canPromote() const {
    // TODO: Проверить, достигла ли пешка последней горизонтали
    return false;
}

ChessPiece* Pawn::promote(PieceType newType) {
    ChessPiece* newPiece = nullptr;

    switch (newType) {
        case PieceType::QUEEN:
            newPiece = new Queen(color, position);
            break;
        case PieceType::ROOK:
            newPiece = new Rook(color, position);
            break;
        case PieceType::BISHOP:
            newPiece = new Bishop(color, position);
            break;
        case PieceType::KNIGHT:
            newPiece = new Knight(color, position);
            break;
        default:
            return nullptr;
    }

    return newPiece;
}

void Pawn::setMoved(bool moved) {
    hasMoved = moved;
}

