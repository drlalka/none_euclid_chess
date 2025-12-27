#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include "../common/Enums.hpp"

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
    std::vector<std::thread> workerThreads;
    std::mutex resultMutex;
    std::atomic<bool> kingInCheck;
    std::atomic<bool> hasLegalMoves;

    bool isSquareUnderAttack(GameBoard* board, int surfaceId, int x, int y, PieceColor defenderColor);
    bool canPieceMove(ChessPiece* piece, GameBoard* board);
    void checkPieceGroup(GameBoard* board, const std::vector<ChessPiece*>& pieces);

public:
    ThreadedVerdictCalculator();
    ~ThreadedVerdictCalculator();

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

