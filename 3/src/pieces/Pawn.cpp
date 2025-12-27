#include "../../include/pieces/Pawn.hpp"
#include "../../include/pieces/Queen.hpp"
#include "../../include/pieces/Rook.hpp"
#include "../../include/pieces/Bishop.hpp"
#include "../../include/pieces/Knight.hpp"
#include "../../include/board/GameBoard.hpp"
#include "../../include/common/Enums.hpp"
#include "../../include/board/surfaces/Surface.hpp"
#include <algorithm>

Pawn::Pawn(PieceColor color, const BoardPosition& position)
    : ChessPiece(color, PieceType::PAWN, position), hasMoved(false)
{
}

std::vector<Move> Pawn::getPossibleMoves(GameBoard* board) const
{
    std::vector<Move> moves;

    int direction = (color == PieceColor::WHITE) ? 1 : -1;
    Surface* surface = board->getSurface(position.getSurfaceId());

    if (!surface)
    {
        return moves;
    }

    int maxY = surface->getHeight() - 1;
    int nextY = position.getY() + direction;

    if (nextY < 0 || nextY > maxY)
    {
        return moves;
    }

    BoardPosition oneStep(position.getSurfaceId(), position.getX(), nextY);
    if (!board->getPieceAt(oneStep))
    {
        moves.push_back(Move(position, oneStep));

        if (!hasMoved)
        {
            int doubleY = position.getY() + 2 * direction;
            if (doubleY >= 0 && doubleY <= maxY)
            {
                BoardPosition twoSteps(position.getSurfaceId(), position.getX(), doubleY);
                BoardPosition between(position.getSurfaceId(), position.getX(), position.getY() + direction);
                if (!board->getPieceAt(twoSteps) && !board->getPieceAt(between))
                {
                    moves.push_back(Move(position, twoSteps));
                }
            }
        }
    }

    int leftX = position.getX() - 1;
    int rightX = position.getX() + 1;
    int maxX = surface->getWidth() - 1;

    if (leftX >= 0)
    {
        BoardPosition leftCapture(position.getSurfaceId(), leftX, nextY);
        ChessPiece* leftPiece = board->getPieceAt(leftCapture);
        if (leftPiece && leftPiece->getColor() != color)
        {
            moves.push_back(Move(position, leftCapture));
        }
    }

    if (rightX <= maxX)
    {
        BoardPosition rightCapture(position.getSurfaceId(), rightX, nextY);
        ChessPiece* rightPiece = board->getPieceAt(rightCapture);
        if (rightPiece && rightPiece->getColor() != color)
        {
            moves.push_back(Move(position, rightCapture));
        }
    }

    auto cell = surface->getCell(position.getX(), position.getY());
    if (cell && cell->hasPortal())
    {
        moves.push_back(Move(position, cell->getPortalDestination()));
    }

    return moves;
}

bool Pawn::isValidMove(const Move& move, GameBoard* board) const
{
    if (move.getFrom() != position)
    {
        return false;
    }
    auto moves = getPossibleMoves(board);
    return std::find(moves.begin(), moves.end(), move) != moves.end();
}

int Pawn::getMaxPortalUse() const
{
    return 1;
}

bool Pawn::canPromote() const
{
    Surface* surface = nullptr;

    if (!surface)
    {
        return false;
    }

    if (color == PieceColor::WHITE)
    {
        return position.getY() == surface->getHeight() - 1;
    }
    else
    {
        return position.getY() == 0;
    }
}

bool Pawn::getHasMoved() const
{
    return hasMoved;
}

void Pawn::setMoved(bool moved)
{
    hasMoved = moved;
}
