#include "../../include/pieces/King.hpp"
#include "../../include/board/GameBoard.hpp"

King::King(PieceColor color, const BoardPosition& position)
    : ChessPiece(color, PieceType::KING, position), hasMoved(false), isInCheck(false) {
}

std::vector<Move> King::getPossibleMoves(GameBoard* board) const {
    std::vector<Move> moves;
    // TODO: Реализовать получение возможных ходов короля
    return moves;
}

bool King::isValidMove(const Move& move, GameBoard* board) const {
    // TODO: Реализовать проверку валидности хода короля
    return false;
}

int King::getMaxPortalUse() const {
    return 1; // Король может использовать 1 портал за ход
}

bool King::canCastle() const {
    return !hasMoved && !isInCheck;
}

void King::setMoved(bool moved) {
    hasMoved = moved;
}

void King::setCheck(bool check) {
    isInCheck = check;
}

