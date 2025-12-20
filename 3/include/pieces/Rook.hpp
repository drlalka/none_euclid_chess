#ifndef ROOK_HPP
#define ROOK_HPP

#include "ChessPiece.hpp"

class Rook : public ChessPiece {
private:
    bool hasMoved;

public:
    Rook(PieceColor color, const BoardPosition& position);

    std::vector<Move> getPossibleMoves(GameBoard* board) const override;
    bool isValidMove(const Move& move, GameBoard* board) const override;
    int getMaxPortalUse() const override;
    void setMoved(bool moved);
};

#endif // ROOK_HPP

