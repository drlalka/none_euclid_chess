#include "../../include/utils/ThreadedVerdictCalculator.hpp"
#include "../../include/board/GameBoard.hpp"

ThreadedVerdictCalculator::ThreadedVerdictCalculator() {
}

ThreadedVerdictCalculator::~ThreadedVerdictCalculator() {
    shutdown();
}

GameVerdict ThreadedVerdictCalculator::calculateGameState(GameBoard* board, PieceColor activeColor) {
    // TODO: Реализовать многопоточный расчет вердикта
    // Каждая группа фигур обрабатывается в отдельном потоке

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

