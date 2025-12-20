#ifndef BISHOP_HPP
#define BISHOP_HPP

#include "ChessPiece.hpp"

class Bishop : public ChessPiece {
public:
    Bishop(PieceColor color, const BoardPosition& position);

    std::vector<Move> getPossibleMoves(GameBoard* board) const override;
    bool isValidMove(const Move& move, GameBoard* board) const override;
    int getMaxPortalUse() const override;
};

#endif // BISHOP_HPP

