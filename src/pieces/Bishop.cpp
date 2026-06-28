#include "../../include/pieces/Bishop.hpp"
#include "../../include/board/GameBoard.hpp"
#include "../../include/board/surfaces/Surface.hpp"
#include <algorithm>
#include <set>

Bishop::Bishop(PieceColor color, const BoardPosition& position)
    : ChessPiece(color, PieceType::BISHOP, position)
{
}

std::vector<Move> Bishop::getPossibleMoves(GameBoard* board) const
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

    Cell::Direction diagonalDirections[4] = {
        Cell::Direction::NORTHEAST,
        Cell::Direction::SOUTHEAST,
        Cell::Direction::SOUTHWEST,
        Cell::Direction::NORTHWEST
    };

    for (auto direction : diagonalDirections)
    {
        std::set<Cell*> visited;
        visited.insert(startCell);

        Cell* current = startCell->getNeighbor(direction);

        while (current != nullptr)
        {
            if (visited.count(current) > 0)
            {
                break;
            }

            visited.insert(current);

            const BoardPosition& newPos = current->getPosition();
            ChessPiece* targetPiece = board->getPieceAt(newPos);

            if (current->hasPortal())
            {
                break;
            }

            if (targetPiece == nullptr)
            {
                moves.push_back(Move(position, newPos));
            }
            else
            {
                if (targetPiece->getColor() != color)
                {
                    moves.push_back(Move(position, newPos));
                }
                break;
            }

            current = current->getNeighbor(direction);
        }
    }

    return moves;
}

bool Bishop::isValidMove(const Move& move, GameBoard* board) const
{
    if (move.getFrom() != position)
    {
        return false;
    }

    auto moves = getPossibleMoves(board);
    return std::find(moves.begin(), moves.end(), move) != moves.end();
}

int Bishop::getMaxPortalUse() const
{
    return 1;
}
