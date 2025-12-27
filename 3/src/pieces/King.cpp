#include "../../include/pieces/King.hpp"
#include "../../include/board/GameBoard.hpp"
#include "../../include/board/surfaces/Surface.hpp"
#include <algorithm>

King::King(PieceColor color, const BoardPosition& position)
    : ChessPiece(color, PieceType::KING, position), hasMoved(false), isInCheck(false)
{
}

std::vector<Move> King::getPossibleMoves(GameBoard* board) const
{
    std::vector<Move> moves;

    Surface* surface = board->getSurface(position.getSurfaceId());
    if (!surface)
    {
        return moves;
    }

    int directions[8][2] = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0},
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };

    for (auto& dir : directions)
    {
        int newX = position.getX() + dir[0];
        int newY = position.getY() + dir[1];

        if (!surface->isValidCoordinate(newX, newY))
        {
            continue;
        }

        BoardPosition newPos(position.getSurfaceId(), newX, newY);

        auto cell = surface->getCell(newX, newY);
        if (!cell) {
            continue;
        }

        ChessPiece* targetPiece = board->getPieceAt(newPos);

        if (cell->hasPortal())
        {
            // TODO: Implement portal logic for King (1 portal per move)
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

    // TODO: Add castling logic if !hasMoved && !isInCheck

    return moves;
}

bool King::isValidMove(const Move& move, GameBoard* board) const
{
    if (move.getFrom() != position)
    {
        return false;
    }

    auto moves = getPossibleMoves(board);
    return std::find(moves.begin(), moves.end(), move) != moves.end();
}

int King::getMaxPortalUse() const
{
    return 1;
}

bool King::canCastle() const
{
    return !hasMoved && !isInCheck;
}

void King::setMoved(bool moved)
{
    hasMoved = moved;
}

void King::setCheck(bool check)
{
    isInCheck = check;
}

bool King::getHasMoved() const
{
    return hasMoved;
}

bool King::getIsInCheck() const
{
    return isInCheck;
}
