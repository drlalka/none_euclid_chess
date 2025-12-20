#ifndef GAMEBOARD_HPP
#define GAMEBOARD_HPP

#include <vector>
#include <map>
#include "surfaces/Surface.hpp"
#include "../common/BoardPosition.hpp"

class ChessPiece;

class GameBoard {
private:
    std::vector<Surface*> surfaces;
    std::map<int, Surface*> surfaceMap;

public:
    GameBoard();
    ~GameBoard();

    Surface* getSurface(int id) const;
    ChessPiece* movePiece(const BoardPosition& from, const BoardPosition& to);
    bool isValidMove(const BoardPosition& from, const BoardPosition& to) const;
    ChessPiece* getPieceAt(const BoardPosition& pos) const;
    void addSurface(Surface* surface);
    std::vector<ChessPiece*> getAllPieces() const;
};

#endif // GAMEBOARD_HPP

