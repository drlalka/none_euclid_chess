#ifndef QUEEN_HPP
#define QUEEN_HPP

#include "ChessPiece.hpp"

class Queen : public ChessPiece {
public:
    Queen(PieceColor color, const BoardPosition& position);

    std::vector<Move> getPossibleMoves(GameBoard* board) const override;
    bool isValidMove(const Move& move, GameBoard* board) const override;
    int getMaxPortalUse() const override;
};

#endif // QUEEN_HPP

