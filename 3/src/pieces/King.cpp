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

    Cell* startCell = surface->getCell(position.getX(), position.getY());
    if (!startCell)
    {
        return moves;
    }

    Cell::Direction allDirections[8] = {
        Cell::Direction::NORTH,
        Cell::Direction::NORTHEAST,
        Cell::Direction::EAST,
        Cell::Direction::SOUTHEAST,
        Cell::Direction::SOUTH,
        Cell::Direction::SOUTHWEST,
        Cell::Direction::WEST,
        Cell::Direction::NORTHWEST
    };

    for (auto direction : allDirections)
    {
        Cell* neighbor = startCell->getNeighbor(direction);
        if (neighbor == nullptr)
        {
            continue;
        }

        const BoardPosition& newPos = neighbor->getPosition();
        ChessPiece* targetPiece = board->getPieceAt(newPos);

        if (neighbor->hasPortal())
        {
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

void King::onMove(const Move& move)
{
    hasMoved = true;
}

void King::beforeMove(Move& move) const
{
    move.setPieceHadMoved(hasMoved);
}

