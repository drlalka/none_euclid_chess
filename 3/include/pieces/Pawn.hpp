#ifndef PAWN_HPP
#define PAWN_HPP

#include "ChessPiece.hpp"

class Pawn : public ChessPiece {
private:
    bool hasMoved;

public:
    Pawn(PieceColor color, const BoardPosition& position);

    std::vector<Move> getPossibleMoves(GameBoard* board) const override;
    bool isValidMove(const Move& move, GameBoard* board) const override;
    int getMaxPortalUse() const override;
    bool canPromote() const;
    ChessPiece* promote(PieceType newType);
    void setMoved(bool moved);
};

#endif // PAWN_HPP

