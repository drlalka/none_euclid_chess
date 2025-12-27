#include "../../include/pieces/Knight.hpp"
#include "../../include/board/GameBoard.hpp"
#include "../../include/board/surfaces/Surface.hpp"
#include <algorithm>

Knight::Knight(PieceColor color, const BoardPosition& position)
    : ChessPiece(color, PieceType::KNIGHT, position)
{
}

std::vector<Move> Knight::getPossibleMoves(GameBoard* board) const
{
    std::vector<Move> moves;

    Surface* surface = board->getSurface(position.getSurfaceId());
    if (!surface)
    {
        return moves;
    }

    int knightMoves[8][2] = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
    };

    for (auto& move : knightMoves)
    {
        int newX = position.getX() + move[0];
        int newY = position.getY() + move[1];

        if (!surface->isValidCoordinate(newX, newY))
        {
            continue;
        }

        auto cell = surface->getCell(newX, newY);
        if (!cell) {
            continue;
        }

        BoardPosition newPos(position.getSurfaceId(), newX, newY);
        ChessPiece* targetPiece = board->getPieceAt(newPos);

        if (cell->hasPortal())
        {
            // TODO: Implement portal logic for Knight (2 portals + 1 cell move)
            continue;
        }

        if (targetPiece == nullptr)
        {
            moves.push_back(Move(position, newPos));
        }
        else if (targetPiece->getColor() != color)
        {
            moves.push_back(Move(position, newPos));
        }
    }

    return moves;
}

bool Knight::isValidMove(const Move& move, GameBoard* board) const
{
    if (move.getFrom() != position)
    {
        return false;
    }

    auto moves = getPossibleMoves(board);
    return std::find(moves.begin(), moves.end(), move) != moves.end();
}

int Knight::getMaxPortalUse() const
{
    return 2;
}
