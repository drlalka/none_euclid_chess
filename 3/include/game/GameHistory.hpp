#ifndef GAMEHISTORY_HPP
#define GAMEHISTORY_HPP

#include <vector>
#include "../common/Move.hpp"

class ChessPiece;

class GameHistory {
private:
    std::vector<Move> moves;
    std::vector<ChessPiece*> capturedPieces;

public:
    GameHistory();
    ~GameHistory();

    void addMove(const Move& move, ChessPiece* capturedPiece = nullptr);
    Move getLastMove() const;
    void undoLastMove();
    const std::vector<Move>& getMoves() const;
    void clear();
};

#endif // GAMEHISTORY_HPP

