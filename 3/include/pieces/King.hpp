#ifndef KING_HPP
#define KING_HPP

#include "ChessPiece.hpp"

class King : public ChessPiece {
private:
    bool hasMoved;
    bool isInCheck;

public:
    King(PieceColor color, const BoardPosition& position);

    std::vector<Move> getPossibleMoves(GameBoard* board) const override;
    bool isValidMove(const Move& move, GameBoard* board) const override;
    int getMaxPortalUse() const override;
    bool canCastle() const;
    void setMoved(bool moved);
    void setCheck(bool check);
};

#endif // KING_HPP

