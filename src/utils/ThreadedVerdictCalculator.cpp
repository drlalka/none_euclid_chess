#include "../../include/utils/ThreadedVerdictCalculator.hpp"
#include "../../include/board/GameBoard.hpp"
#include "../../include/pieces/ChessPiece.hpp"
#include <algorithm>
#include <chrono>
#include <iostream>

ThreadedVerdictCalculator::ThreadedVerdictCalculator()
    : numThreads(4), kingInCheck(false), hasLegalMoves(false), stopThreads(false) {
    initializeThreadPool();
}

ThreadedVerdictCalculator::~ThreadedVerdictCalculator() {
    shutdown();
}

void ThreadedVerdictCalculator::initializeThreadPool() {
    stopThreads.store(false);
    for (size_t i = 0; i < numThreads; i++) {
        workerThreads.emplace_back(&ThreadedVerdictCalculator::workerLoop, this);
    }
}

void ThreadedVerdictCalculator::workerLoop() {
    while (true) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(jobMutex);
            jobCV.wait(lock, [this] { return stopThreads.load() || !jobQueue.empty(); });

            if (stopThreads.load() && jobQueue.empty()) {
                return;
            }

            if (!jobQueue.empty()) {
                job = std::move(jobQueue.back());
                jobQueue.pop_back();
            }
        }

        if (job) {
            job();
        }
    }
}

void ThreadedVerdictCalculator::setNumThreads(size_t threads) {
    if (threads == numThreads) return;

    shutdown();
    numThreads = std::max(size_t(1), threads);
    initializeThreadPool();
}

size_t ThreadedVerdictCalculator::getNumThreads() const {
    return numThreads;
}

bool ThreadedVerdictCalculator::isSquareUnderAttack(GameBoard* board, const BoardPosition& position, PieceColor defenderColor) {
    auto allPieces = board->getAllPieces();

    for (auto* piece : allPieces) {
        if (piece->getColor() == defenderColor) {
            continue;
        }

        auto moves = piece->getPossibleMoves(board);
        for (const auto& move : moves) {
            if (move.getTo() == position) {
                return true;
            }
        }
    }

    return false;
}

void ThreadedVerdictCalculator::checkPieceGroup(GameBoard* board, const std::vector<ChessPiece*>& pieces) {
    for (auto* piece : pieces) {
        auto moves = piece->getPossibleMoves(board);
        if (!moves.empty()) {
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

    const BoardPosition& kingPos = king->getPosition();
    if (isSquareUnderAttack(board, kingPos, activeColor)) {
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

    size_t effectiveThreads = std::min(numThreads, playerPieces.size());

    if (effectiveThreads == 1 || playerPieces.size() < 50) {
        for (auto* piece : playerPieces) {
            auto moves = piece->getPossibleMoves(board);
            if (!moves.empty()) {
                hasLegalMoves.store(true);
                break;
            }
        }
    } else {
        size_t minPiecesPerThread = 20;
        size_t piecesPerThread = std::max(minPiecesPerThread,
                                         (playerPieces.size() + effectiveThreads - 1) / effectiveThreads);

        std::mutex completionMutex;
        std::condition_variable completionCV;
        size_t completedJobs = 0;
        size_t totalJobs = (playerPieces.size() + piecesPerThread - 1) / piecesPerThread;

        {
            std::lock_guard<std::mutex> lock(jobMutex);
            jobQueue.clear();

            for (size_t i = 0; i < playerPieces.size(); i += piecesPerThread) {
                size_t end = std::min(i + piecesPerThread, playerPieces.size());

                jobQueue.push_back([this, board, &playerPieces, i, end, &completionMutex, &completionCV, &completedJobs]() {
                    for (size_t j = i; j < end && !hasLegalMoves.load(); ++j) {
                        auto moves = playerPieces[j]->getPossibleMoves(board);
                        if (!moves.empty()) {
                            hasLegalMoves.store(true);
                            break;
                        }
                    }

                    std::lock_guard<std::mutex> lock(completionMutex);
                    completedJobs++;
                    completionCV.notify_one();
                });
            }
        }

        jobCV.notify_all();

        std::unique_lock<std::mutex> completionLock(completionMutex);
        completionCV.wait(completionLock, [&completedJobs, totalJobs]() {
            return completedJobs >= totalJobs;
        });
    }

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
    stopThreads.store(true);
    jobCV.notify_all();

    for (auto& thread : workerThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    workerThreads.clear();
}

