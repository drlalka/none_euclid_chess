#include "../../include/pieces/Pawn.hpp"
#include "../../include/pieces/Queen.hpp"
#include "../../include/pieces/Rook.hpp"
#include "../../include/pieces/Bishop.hpp"
#include "../../include/pieces/Knight.hpp"
#include "../../include/board/GameBoard.hpp"
#include "../../include/common/Enums.hpp"
#include "../../include/board/surfaces/Surface.hpp"
#include <algorithm>
#include <set>

Pawn::Pawn(PieceColor color, const BoardPosition& position)
    : ChessPiece(color, PieceType::PAWN, position), hasMoved(false)
{
}

std::vector<Move> Pawn::getPossibleMoves(GameBoard* board) const
{
    std::vector<Move> moves;

    Surface* surface = board->getSurface(position.getSurfaceId());
    if (!surface) return moves;

    Cell* startCell = surface->getCell(position.getX(), position.getY());
    if (!startCell) return moves;

    Cell::Direction forward = (color == PieceColor::WHITE) ? Cell::Direction::NORTH : Cell::Direction::SOUTH;
    Cell::Direction forwardLeft = (color == PieceColor::WHITE) ? Cell::Direction::NORTHWEST : Cell::Direction::SOUTHWEST;
    Cell::Direction forwardRight = (color == PieceColor::WHITE) ? Cell::Direction::NORTHEAST : Cell::Direction::SOUTHEAST;

    Cell* oneStepCell = startCell->getNeighbor(forward);
    if (oneStepCell != nullptr) {
        const BoardPosition& oneStepPos = oneStepCell->getPosition();
        ChessPiece* pieceAtOneStep = board->getPieceAt(oneStepPos);

        if (pieceAtOneStep == nullptr) {
            if (oneStepCell->hasPortal()) {
                PortalLink* portal = oneStepCell->getPortalLink();
                if (portal && portal->isActive()) {
                    BoardPosition portalDest = portal->getDestination();
                    Move move(position, portalDest);
                    move.setPortalUsed(true);
                    moves.push_back(move);
                }
            } else {
                moves.push_back(Move(position, oneStepPos));

                if (!hasMoved) {
                    Cell* twoStepCell = oneStepCell->getNeighbor(forward);
                    if (twoStepCell != nullptr) {
                        const BoardPosition& twoStepPos = twoStepCell->getPosition();
                        ChessPiece* pieceAtTwoSteps = board->getPieceAt(twoStepPos);
                        if (pieceAtTwoSteps == nullptr) {
                            moves.push_back(Move(position, twoStepPos));
                        }
                    }
                }
            }
        }
    }

    Cell* leftDiagonalCell = startCell->getNeighbor(forwardLeft);
    if (leftDiagonalCell != nullptr) {
        const BoardPosition& leftPos = leftDiagonalCell->getPosition();
        ChessPiece* leftPiece = board->getPieceAt(leftPos);

        if (leftPiece != nullptr && leftPiece->getColor() != color) {
            moves.push_back(Move(position, leftPos));
        } else if (leftPiece == nullptr && leftDiagonalCell->hasPortal()) {
            PortalLink* portal = leftDiagonalCell->getPortalLink();
            if (portal && portal->isActive()) {
                BoardPosition portalDest = portal->getDestination();
                Move move(position, portalDest);
                move.setPortalUsed(true);
                moves.push_back(move);
            }
        }
    }

    Cell* rightDiagonalCell = startCell->getNeighbor(forwardRight);
    if (rightDiagonalCell != nullptr) {
        const BoardPosition& rightPos = rightDiagonalCell->getPosition();
        ChessPiece* rightPiece = board->getPieceAt(rightPos);

        if (rightPiece != nullptr && rightPiece->getColor() != color) {
            moves.push_back(Move(position, rightPos));
        } else if (rightPiece == nullptr && rightDiagonalCell->hasPortal()) {
            PortalLink* portal = rightDiagonalCell->getPortalLink();
            if (portal && portal->isActive()) {
                BoardPosition portalDest = portal->getDestination();
                Move move(position, portalDest);
                move.setPortalUsed(true);
                moves.push_back(move);
            }
        }
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

void Pawn::onMove(const Move& move)
{
    hasMoved = true;
}

void Pawn::beforeMove(Move& move) const
{
    move.setPieceHadMoved(hasMoved);
}

