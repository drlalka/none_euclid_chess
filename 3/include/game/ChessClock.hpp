#ifndef CHESSCLOCK_HPP
#define CHESSCLOCK_HPP

#include <chrono>
#include "../common/Enums.hpp"

using Duration = std::chrono::milliseconds;

class ChessClock {
private:
    Duration whiteTime;
    Duration blackTime;
    PieceColor activeColor;
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    bool isRunning;

public:
    ChessClock();

    void startTimer(PieceColor color);
    void restartTimer();
    void pauseTimer();
    Duration getTime(PieceColor color) const;
};

#endif // CHESSCLOCK_HPP

