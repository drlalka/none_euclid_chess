#ifndef CHESSPIECE_HPP
#define CHESSPIECE_HPP

#include <vector>
#include "../common/Enums.hpp"
#include "../common/BoardPosition.hpp"
#include "../common/Move.hpp"

class GameBoard;

class ChessPiece {
protected:
    PieceColor color;
    PieceType type;
    BoardPosition position;

public:
    ChessPiece(PieceColor color, PieceType type, const BoardPosition& position);
    virtual ~ChessPiece() = default;

    virtual std::vector<Move> getPossibleMoves(GameBoard* board) const = 0;
    virtual bool isValidMove(const Move& move, GameBoard* board) const = 0;
    virtual int getMaxPortalUse() const = 0;

    PieceType getType() const;
    int getValue() const;
    PieceColor getColor() const;
    const BoardPosition& getPosition() const;
    void moveTo(const BoardPosition& newPosition);
};

#endif // CHESSPIECE_HPP

