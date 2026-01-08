#include "../../include/pieces/Knight.hpp"
#include "../../include/board/GameBoard.hpp"
#include "../../include/board/surfaces/Surface.hpp"
#include <algorithm>
#include <set>

Knight::Knight(PieceColor color, const BoardPosition& position)
    : ChessPiece(color, PieceType::KNIGHT, position)
{
}

std::vector<Move> Knight::getPossibleMoves(GameBoard* board) const
{
    std::vector<Move> moves;
    std::set<std::pair<int, std::pair<int, int>>> visited;

    auto addKnightMoves = [&](const BoardPosition& from, int portalsUsed, auto& self) -> void {
        auto visitKey = std::make_pair(from.getSurfaceId(), std::make_pair(from.getX(), from.getY()));
        if (visited.count(visitKey)) return;
        visited.insert(visitKey);

        Surface* surface = board->getSurface(from.getSurfaceId());
        if (!surface) return;

        Cell* startCell = surface->getCell(from.getX(), from.getY());
        if (!startCell) return;

        struct KnightPath {
            Cell::Direction first;
            Cell::Direction second;
            Cell::Direction third;
        };

        KnightPath knightPaths[8] = {
            {Cell::Direction::NORTH, Cell::Direction::NORTH, Cell::Direction::EAST},
            {Cell::Direction::NORTH, Cell::Direction::NORTH, Cell::Direction::WEST},
            {Cell::Direction::SOUTH, Cell::Direction::SOUTH, Cell::Direction::EAST},
            {Cell::Direction::SOUTH, Cell::Direction::SOUTH, Cell::Direction::WEST},
            {Cell::Direction::EAST, Cell::Direction::EAST, Cell::Direction::NORTH},
            {Cell::Direction::EAST, Cell::Direction::EAST, Cell::Direction::SOUTH},
            {Cell::Direction::WEST, Cell::Direction::WEST, Cell::Direction::NORTH},
            {Cell::Direction::WEST, Cell::Direction::WEST, Cell::Direction::SOUTH}
        };


        for (auto& path : knightPaths) {
            Cell* step1 = startCell->getNeighbor(path.first);
            if (step1 == nullptr) continue;

            Cell* step2 = step1->getNeighbor(path.second);
            if (step2 == nullptr) continue;

            Cell* destination = step2->getNeighbor(path.third);
            if (destination == nullptr) continue;

            const BoardPosition& newPos = destination->getPosition();
            ChessPiece* targetPiece = board->getPieceAt(newPos);

            if (targetPiece == nullptr || targetPiece->getColor() != color) {
                Move move(position, newPos);
                move.setPortalUsed(portalsUsed > 0);
                moves.push_back(move);
            }
        }
    };

    addKnightMoves(position, 0, addKnightMoves);
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
