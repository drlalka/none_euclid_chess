#include "../../include/pieces/Queen.hpp"
#include "../../include/board/GameBoard.hpp"

Queen::Queen(PieceColor color, const BoardPosition& position)
    : ChessPiece(color, PieceType::QUEEN, position) {
}

std::vector<Move> Queen::getPossibleMoves(GameBoard* board) const {
    std::vector<Move> moves;
    // TODO: Реализовать получение возможных ходов ферзя
    return moves;
}

bool Queen::isValidMove(const Move& move, GameBoard* board) const {
    // TODO: Реализовать проверку валидности хода ферзя
    return false;
}

int Queen::getMaxPortalUse() const {
    return 1; // Ферзь может использовать 1 портал за ход
}

