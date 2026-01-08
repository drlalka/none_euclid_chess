#include "../../include/utils/ThreadedVerdictCalculator.hpp"
#include "../../include/board/GameBoard.hpp"
#include "../../include/pieces/ChessPiece.hpp"
#include <algorithm>
#include <chrono>
#include <iostream>

ThreadedVerdictCalculator::ThreadedVerdictCalculator()
    : numThreads(4), kingInCheck(false), hasLegalMoves(false), stopThreads(false), activeJobs(0) {
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
            activeJobs.fetch_sub(1);
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

    size_t piecesPerThread = std::max(MIN_PIECES_PER_THREAD, playerPieces.size() / numThreads);

    size_t numJobs = 0;
    {
        std::lock_guard<std::mutex> lock(jobMutex);
        jobQueue.clear();
        activeJobs.store(0);

        for (size_t i = 0; i < playerPieces.size(); i += piecesPerThread) {
            size_t end = std::min(i + piecesPerThread, playerPieces.size());
            std::vector<ChessPiece*> group(playerPieces.begin() + i, playerPieces.begin() + end);

            activeJobs.fetch_add(1);
            jobQueue.push_back([this, board, group]() {
                this->checkPieceGroup(board, group);
            });
            numJobs++;
        }
    }

    jobCV.notify_all();

    while (activeJobs.load() > 0) {
        std::this_thread::sleep_for(std::chrono::microseconds(100));
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

