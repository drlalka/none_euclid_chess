#include "../../include/pieces/Knight.hpp"
#include "../../include/board/GameBoard.hpp"

Knight::Knight(PieceColor color, const BoardPosition& position)
    : ChessPiece(color, PieceType::KNIGHT, position) {
}

std::vector<Move> Knight::getPossibleMoves(GameBoard* board) const {
    std::vector<Move> moves;
    // TODO: Реализовать получение возможных ходов коня
    return moves;
}

bool Knight::isValidMove(const Move& move, GameBoard* board) const {
    // TODO: Реализовать проверку валидности хода коня
    return false;
}

int Knight::getMaxPortalUse() const {
    return 2; // Конь может использовать 2 портала за ход
}

