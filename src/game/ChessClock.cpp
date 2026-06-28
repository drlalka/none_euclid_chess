#include "../../include/game/ChessClock.hpp"

ChessClock::ChessClock()
    : whiteTime(0), blackTime(0), activeColor(PieceColor::WHITE), isRunning(false) {
}

void ChessClock::startTimer(PieceColor color) {
    if (isRunning && activeColor != color) {
        pauseTimer();
    }
    activeColor = color;
    startTime = std::chrono::steady_clock::now();
    isRunning = true;
}

void ChessClock::restartTimer() {
    whiteTime = Duration(0);
    blackTime = Duration(0);
    activeColor = PieceColor::WHITE;
    isRunning = false;
}

void ChessClock::pauseTimer() {
    if (!isRunning) {
        return;
    }

    auto endTime = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<Duration>(endTime - startTime);

    if (activeColor == PieceColor::WHITE) {
        whiteTime += elapsed;
    } else {
        blackTime += elapsed;
    }

    isRunning = false;
}

Duration ChessClock::getTime(PieceColor color) const {
    Duration currentTime = (color == PieceColor::WHITE) ? whiteTime : blackTime;

    if (isRunning && activeColor == color) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<Duration>(now - startTime);
        currentTime += elapsed;
    }

    return currentTime;
}

