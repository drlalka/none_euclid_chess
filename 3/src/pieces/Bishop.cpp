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

    int directions[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    for (auto& dir : directions)
    {
        int dx = dir[0];
        int dy = dir[1];

        std::set<std::pair<int, int>> visited;
        visited.insert({position.getX(), position.getY()});

        int currentX = position.getX();
        int currentY = position.getY();

        while (true)
        {
            currentX += dx;
            currentY += dy;

            if (!surface->isValidCoordinate(currentX, currentY))
            {
                break;
            }

            int normalizedX = currentX;
            if (surface->getCell(currentX, currentY) == nullptr && surface->getWidth() > 0) {
                normalizedX = ((currentX % surface->getWidth()) + surface->getWidth()) % surface->getWidth();
                if (surface->getCell(normalizedX, currentY) == nullptr) {
                    break;
                }
            }

            if (visited.count({normalizedX, currentY}) > 0)
            {
                break;
            }

            visited.insert({normalizedX, currentY});

            BoardPosition newPos(position.getSurfaceId(), normalizedX, currentY);
            ChessPiece* targetPiece = board->getPieceAt(newPos);

            auto cell = surface->getCell(normalizedX, currentY);
            if (cell && cell->hasPortal())
            {
                // TODO: Implement portal logic for Bishop (1 portal per move)
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
