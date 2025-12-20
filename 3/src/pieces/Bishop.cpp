#include "../../include/pieces/Bishop.hpp"
#include "../../include/board/GameBoard.hpp"

Bishop::Bishop(PieceColor color, const BoardPosition& position)
    : ChessPiece(color, PieceType::BISHOP, position) {
}

std::vector<Move> Bishop::getPossibleMoves(GameBoard* board) const {
    std::vector<Move> moves;
    // TODO: Реализовать получение возможных ходов слона
    return moves;
}

bool Bishop::isValidMove(const Move& move, GameBoard* board) const {
    // TODO: Реализовать проверку валидности хода слона
    return false;
}

int Bishop::getMaxPortalUse() const {
    return 1; // Слон может использовать 1 портал за ход
}

