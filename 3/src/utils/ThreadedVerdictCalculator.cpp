#include "../../include/utils/ThreadedVerdictCalculator.hpp"
#include "../../include/board/GameBoard.hpp"
#include "../../include/pieces/ChessPiece.hpp"
#include "../../include/pieces/King.hpp"
#include <algorithm>

ThreadedVerdictCalculator::ThreadedVerdictCalculator()
    : kingInCheck(false), hasLegalMoves(false) {
}

ThreadedVerdictCalculator::~ThreadedVerdictCalculator() {
    shutdown();
}

bool ThreadedVerdictCalculator::isSquareUnderAttack(GameBoard* board, int surfaceId, int x, int y, PieceColor defenderColor) {
    auto allPieces = board->getAllPieces();

    for (auto* piece : allPieces) {
        if (piece->getColor() == defenderColor) {
            continue;
        }

        auto moves = piece->getPossibleMoves(board);
        for (const auto& move : moves) {
            if (move.getTo().getSurfaceId() == surfaceId &&
                move.getTo().getX() == x &&
                move.getTo().getY() == y) {
                return true;
            }
        }
    }

    return false;
}

bool ThreadedVerdictCalculator::canPieceMove(ChessPiece* piece, GameBoard* board) {
    auto moves = piece->getPossibleMoves(board);
    return !moves.empty();
}

void ThreadedVerdictCalculator::checkPieceGroup(GameBoard* board, const std::vector<ChessPiece*>& pieces) {
    for (auto* piece : pieces) {
        if (canPieceMove(piece, board)) {
            hasLegalMoves.store(true);
            return;
        }
    }
}

GameVerdict ThreadedVerdictCalculator::calculateGameState(GameBoard* board, PieceColor activeColor) {
    if (!board) {
        return GameVerdict::NONE;
    }

    kingInCheck.store(false);
    hasLegalMoves.store(false);

    auto allPieces = board->getAllPieces();

    ChessPiece* king = nullptr;
    for (auto* piece : allPieces) {
        if (piece->getColor() == activeColor && piece->getType() == PieceType::KING) {
            king = piece;
            break;
        }
    }

    if (!king) {
        return GameVerdict::NONE;
    }

    const auto& kingPos = king->getPosition();
    if (isSquareUnderAttack(board, kingPos.getSurfaceId(), kingPos.getX(), kingPos.getY(), activeColor)) {
        kingInCheck.store(true);
    }

    std::vector<ChessPiece*> playerPieces;
    for (auto* piece : allPieces) {
        if (piece->getColor() == activeColor) {
            playerPieces.push_back(piece);
        }
    }

    if (playerPieces.empty()) {
        return GameVerdict::NONE;
    }

    size_t piecesPerThread = std::max(size_t(1), playerPieces.size() / 4);
    workerThreads.clear();

    for (size_t i = 0; i < playerPieces.size(); i += piecesPerThread) {
        size_t end = std::min(i + piecesPerThread, playerPieces.size());
        std::vector<ChessPiece*> group(playerPieces.begin() + i, playerPieces.begin() + end);

        workerThreads.emplace_back([this, board, group]() {
            this->checkPieceGroup(board, group);
        });
    }

    for (auto& thread : workerThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    workerThreads.clear();

    bool inCheck = kingInCheck.load();
    bool hasMoves = hasLegalMoves.load();

    if (inCheck) {
        if (!hasMoves) {
            return GameVerdict::CHECKMATE;
        }
        return GameVerdict::CHECK;
    }

    if (!hasMoves) {
        return GameVerdict::STALEMATE;
    }

    return GameVerdict::NONE;
}

void ThreadedVerdictCalculator::shutdown() {
    for (auto& thread : workerThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    workerThreads.clear();
}

