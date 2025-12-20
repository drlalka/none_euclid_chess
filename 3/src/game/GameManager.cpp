#include "../../include/game/GameManager.hpp"
#include "../../include/common/Move.hpp"
#include "../../include/pieces/ChessPiece.hpp"
#include <fstream>
#include <iostream>

GameManager::GameManager()
    : currentState(GameState::NO_GAME),
      currentVerdict(GameVerdict::NONE),
      currentPlayerId(0),
      capturedFigure(nullptr) {
    players[0] = nullptr;
    players[1] = nullptr;
}

GameManager::~GameManager() {
    // Players are managed externally
}

bool GameManager::makeMove(const Move& move) {
    if (currentState != GameState::IN_PROGRESS) {
        return false;
    }

    if (!validateMove(move)) {
        return false;
    }

    capturedFigure = gameBoard.movePiece(move.getFrom(), move.getTo());
    gameHistory.addMove(move, capturedFigure);

    // Switch player
    currentPlayerId = 1 - currentPlayerId;

    // Calculate verdict
    currentVerdict = calculateVerdict();

    if (currentVerdict == GameVerdict::CHECKMATE || currentVerdict == GameVerdict::STALEMATE) {
        currentState = GameState::FINISHED;
    }

    return true;
}

bool GameManager::validateMove(const Move& move) {
    // TODO: Реализовать полную валидацию хода
    return true;
}

GameVerdict GameManager::calculateVerdict() {
    if (players[currentPlayerId] == nullptr) {
        return GameVerdict::NONE;
    }

    return verdictCalculator.calculateGameState(&gameBoard, players[currentPlayerId]->getColor());
}

void GameManager::startNewGame(Player* player1, Player* player2) {
    players[0] = player1;
    players[1] = player2;
    currentPlayerId = 0;
    currentState = GameState::IN_PROGRESS;
    currentVerdict = GameVerdict::NONE;
    capturedFigure = nullptr;

    gameHistory.clear();
    chessClock.restartTimer();
    chessClock.startTimer(PieceColor::WHITE);
}

void GameManager::saveGame(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл для сохранения: " << filename << std::endl;
        return;
    }

    // TODO: Реализовать сохранение партии в файл

    file.close();
}

void GameManager::loadGame(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл для загрузки: " << filename << std::endl;
        return;
    }

    // TODO: Реализовать загрузку партии из файла

    file.close();
}

void GameManager::showHint(GameVerdict verdict) {
    switch (verdict) {
        case GameVerdict::CHECK:
            std::cout << "Шах!" << std::endl;
            break;
        case GameVerdict::CHECKMATE:
            std::cout << "Мат!" << std::endl;
            break;
        case GameVerdict::STALEMATE:
            std::cout << "Пат!" << std::endl;
            break;
        case GameVerdict::NONE:
            // Нет подсказки
            break;
    }
}

ChessPiece* GameManager::getCapturedFigure() const {
    return capturedFigure;
}

