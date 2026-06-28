#pragma once

#include <chrono>
#include "../common/Enums.hpp"

using Duration = std::chrono::milliseconds;

/**
 * @brief Manages game time for both players
 */
class ChessClock {
private:
    Duration whiteTime;
    Duration blackTime;
    PieceColor activeColor;
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    bool isRunning;

public:
    /**
     * @brief Default constructor - initializes clock to zero
     */
    ChessClock();

    /**
     * @brief Starts timer for specific color
     * @param color Color of active player
     */
    void startTimer(PieceColor color);

    /**
     * @brief Restarts timer from zero
     */
    void restartTimer();

    /**
     * @brief Pauses current timer
     */
    void pauseTimer();

    /**
     * @brief Gets elapsed time for player
     * @param color Player color
     * @return Elapsed time in milliseconds
     */
    Duration getTime(PieceColor color) const;
};


