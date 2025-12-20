#ifndef THREADEDVERDICTCALCULATOR_HPP
#define THREADEDVERDICTCALCULATOR_HPP

#include <vector>
#include <thread>
#include <mutex>
#include "../common/Enums.hpp"

class GameBoard;

class ThreadedVerdictCalculator {
private:
    std::vector<std::thread> workerThreads;
    std::mutex resultMutex;

public:
    ThreadedVerdictCalculator();
    ~ThreadedVerdictCalculator();

    GameVerdict calculateGameState(GameBoard* board, PieceColor activeColor);
    void shutdown();
};

#endif // THREADEDVERDICTCALCULATOR_HPP

