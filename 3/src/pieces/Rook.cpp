#include "../../include/pieces/Rook.hpp"
#include "../../include/board/GameBoard.hpp"

Rook::Rook(PieceColor color, const BoardPosition& position)
    : ChessPiece(color, PieceType::ROOK, position), hasMoved(false) {
}

std::vector<Move> Rook::getPossibleMoves(GameBoard* board) const {
    std::vector<Move> moves;
    // TODO: Реализовать получение возможных ходов ладьи
    return moves;
}

bool Rook::isValidMove(const Move& move, GameBoard* board) const {
    // TODO: Реализовать проверку валидности хода ладьи
    return false;
}

int Rook::getMaxPortalUse() const {
    return -1; // Ладья может использовать неограниченное количество порталов
}

void Rook::setMoved(bool moved) {
    hasMoved = moved;
}

