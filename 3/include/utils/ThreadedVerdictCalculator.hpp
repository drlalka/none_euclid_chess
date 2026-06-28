#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <functional>
#include "../common/Enums.hpp"
#include "../common/BoardPosition.hpp"

class GameBoard;
class ChessPiece;

/**
 * @brief Multithreaded verdict calculator for chess game state
 *
 * Calculates game verdict using parallel processing
 * Each piece type is checked in separate thread
 */
class ThreadedVerdictCalculator {
private:
    static constexpr size_t DEFAULT_NUM_THREADS = 4;
    static constexpr size_t MIN_PIECES_PER_THREAD = 1;

    size_t numThreads;
    std::vector<std::thread> workerThreads;
    std::atomic<bool> kingInCheck;
    std::atomic<bool> hasLegalMoves;
    std::atomic<bool> stopThreads;

    std::mutex jobMutex;
    std::condition_variable jobCV;
    std::vector<std::function<void()>> jobQueue;

    void workerLoop();
    void initializeThreadPool();
    bool isSquareUnderAttack(GameBoard* board, const BoardPosition& position, PieceColor defenderColor);
    void checkPieceGroup(GameBoard* board, const std::vector<ChessPiece*>& pieces);

public:
    ThreadedVerdictCalculator();
    ~ThreadedVerdictCalculator();

    /**
     * @brief Sets number of worker threads
     * @param threads Number of threads (minimum 1)
     */
    void setNumThreads(size_t threads);

    /**
     * @brief Gets current number of worker threads
     * @return Number of threads
     */
    size_t getNumThreads() const;

    /**
     * @brief Calculates game verdict using multithreading
     * @param board Game board pointer
     * @param activeColor Color of active player
     * @return GameVerdict (CHECK, CHECKMATE, STALEMATE, NONE)
     */
    GameVerdict calculateGameState(GameBoard* board, PieceColor activeColor);

    /**
     * @brief Shuts down all worker threads
     */
    void shutdown();
};

