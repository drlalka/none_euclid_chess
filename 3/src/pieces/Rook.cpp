#include "../../include/pieces/Rook.hpp"
#include "../../include/board/GameBoard.hpp"
#include "../../include/board/surfaces/Surface.hpp"
#include <algorithm>
#include <set>

Rook::Rook(PieceColor color, const BoardPosition& position)
    : ChessPiece(color, PieceType::ROOK, position), hasMoved(false)
{
}

void exploreDirectionRecursive(
    const BoardPosition& startPos,
    const BoardPosition& rookOriginalPos,
    Cell::Direction direction,
    GameBoard* board,
    PieceColor rookColor,
    std::vector<Move>& moves,
    std::set<std::pair<int, std::pair<int, int>>>& visited,
    bool usedPortal
) {
    Surface* surface = board->getSurface(startPos.getSurfaceId());
    if (!surface) return;

    Cell* currentCell = surface->getCell(startPos.getX(), startPos.getY());
    if (!currentCell) return;

    Cell* next = currentCell->getNeighbor(direction);

    while (next != nullptr) {
        const BoardPosition& nextPos = next->getPosition();

        if (nextPos == rookOriginalPos) break;

        auto visitKey = std::make_pair(nextPos.getSurfaceId(), std::make_pair(nextPos.getX(), nextPos.getY()));

        if (visited.count(visitKey)) break;
        visited.insert(visitKey);

        ChessPiece* piece = board->getPieceAt(nextPos);

        if (next->hasPortal()) {
            PortalLink* portal = next->getPortalLink();
            if (portal && portal->isActive()) {
                BoardPosition portalExit = portal->getDestination();
                Move move(rookOriginalPos, portalExit);
                move.setPortalUsed(true);
                moves.push_back(move);

                exploreDirectionRecursive(portalExit, rookOriginalPos, direction, board, rookColor, moves, visited, true);
            }
            break;
        }

        if (piece == nullptr) {
            Move move(rookOriginalPos, nextPos);
            move.setPortalUsed(usedPortal);
            moves.push_back(move);
        } else {
            if (piece->getColor() != rookColor) {
                Move move(rookOriginalPos, nextPos);
                move.setPortalUsed(usedPortal);
                moves.push_back(move);
            }
            break;
        }

        next = next->getNeighbor(direction);
    }
}

std::vector<Move> Rook::getPossibleMoves(GameBoard* board) const
{
    std::vector<Move> moves;
    std::set<std::pair<int, std::pair<int, int>>> uniqueDestinations;

    Cell::Direction directions[4] = {
        Cell::Direction::NORTH,
        Cell::Direction::SOUTH,
        Cell::Direction::EAST,
        Cell::Direction::WEST
    };

    for (auto direction : directions) {
        std::set<std::pair<int, std::pair<int, int>>> visited;
        std::vector<Move> dirMoves;
        exploreDirectionRecursive(position, position, direction, board, color, dirMoves, visited, false);

        for (const auto& move : dirMoves) {
            auto destKey = std::make_pair(move.getTo().getSurfaceId(),
                                         std::make_pair(move.getTo().getX(), move.getTo().getY()));
            if (uniqueDestinations.insert(destKey).second) {
                moves.push_back(move);
            }
        }
    }

    return moves;
}

bool Rook::isValidMove(const Move& move, GameBoard* board) const
{
    if (move.getFrom() != position)
    {
        return false;
    }

    auto moves = getPossibleMoves(board);
    return std::find(moves.begin(), moves.end(), move) != moves.end();
}

int Rook::getMaxPortalUse() const
{
    return -1;
}

void Rook::setMoved(bool moved)
{
    hasMoved = moved;
}

bool Rook::getHasMoved() const
{
    return hasMoved;
}

void Rook::onMove(const Move& move)
{
    hasMoved = true;
}

void Rook::beforeMove(Move& move) const
{
    move.setPieceHadMoved(hasMoved);
}

