#ifndef KNIGHT_HPP
#define KNIGHT_HPP

#include "ChessPiece.hpp"

class Knight : public ChessPiece {
public:
    Knight(PieceColor color, const BoardPosition& position);

    std::vector<Move> getPossibleMoves(GameBoard* board) const override;
    bool isValidMove(const Move& move, GameBoard* board) const override;
    int getMaxPortalUse() const override;
};

#endif // KNIGHT_HPP

