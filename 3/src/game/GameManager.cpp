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
}

bool GameManager::makeMove(const Move& move) {
    if (currentState != GameState::IN_PROGRESS) {
        return false;
    }

    if (!validateMove(move)) {
        return false;
    }

    Move moveObj = move;
    auto captured = gameBoard.movePiece(moveObj);
    gameHistory.addMove(moveObj, std::move(captured));

    currentPlayerId = 1 - currentPlayerId;

    currentVerdict = calculateVerdict();

    if (currentVerdict == GameVerdict::CHECKMATE || currentVerdict == GameVerdict::STALEMATE) {
        currentState = GameState::FINISHED;
    }

    return true;
}

bool GameManager::validateMove(const Move& move) {
    const BoardPosition& from = move.getFrom();
    const BoardPosition& to = move.getTo();

    ChessPiece* piece = gameBoard.getPieceAt(from);
    if (piece == nullptr) {
        return false;
    }

    if (players[currentPlayerId] == nullptr) {
        return false;
    }

    if (piece->getColor() != players[currentPlayerId]->getColor()) {
        return false;
    }

    if (from == to) {
        return false;
    }

    if (!piece->isValidMove(move, &gameBoard)) {
        return false;
    }

    if (!isMoveLegalConsideringCheck(move)) {
        return false;
    }

    return true;
}

bool GameManager::isMoveLegalConsideringCheck(const Move& move) {
    const BoardPosition& from = move.getFrom();
    const BoardPosition& to = move.getTo();

    ChessPiece* movingPiece = gameBoard.getPieceAt(from);
    if (!movingPiece) {
        return false;
    }

    PieceColor playerColor = movingPiece->getColor();

    ChessPiece* capturedPiece = gameBoard.getPieceAt(to);
    std::unique_ptr<ChessPiece> capturedHolder;

    if (capturedPiece) {
        Surface* surface = gameBoard.getSurface(to.getSurfaceId());
        if (surface) {
            capturedHolder = surface->removePiece(to.getX(), to.getY());
        }
    }

    Surface* fromSurface = gameBoard.getSurface(from.getSurfaceId());
    Surface* toSurface = gameBoard.getSurface(to.getSurfaceId());

    if (!fromSurface || !toSurface) {
        if (capturedHolder) {
            toSurface->addPiece(std::move(capturedHolder), to.getX(), to.getY());
        }
        return false;
    }

    auto movedPiece = fromSurface->removePiece(from.getX(), from.getY());
    if (!movedPiece) {
        if (capturedHolder) {
            toSurface->addPiece(std::move(capturedHolder), to.getX(), to.getY());
        }
        return false;
    }

    BoardPosition oldPosition = movedPiece->getPosition();
    movedPiece->moveTo(to);
    toSurface->addPiece(std::move(movedPiece), to.getX(), to.getY());

    bool isKingInCheck = false;
    auto allPieces = gameBoard.getAllPieces();

    ChessPiece* king = nullptr;
    for (auto* p : allPieces) {
        if (p->getColor() == playerColor && p->getType() == PieceType::KING) {
            king = p;
            break;
        }
    }

    if (king) {
        const BoardPosition& kingPos = king->getPosition();

        for (auto* enemyPiece : allPieces) {
            if (enemyPiece->getColor() == playerColor) {
                continue;
            }

            auto moves = enemyPiece->getPossibleMoves(&gameBoard);
            for (const auto& enemyMove : moves) {
                if (enemyMove.getTo() == kingPos) {
                    isKingInCheck = true;
                    break;
                }
            }

            if (isKingInCheck) {
                break;
            }
        }
    }

    auto pieceToRestore = toSurface->removePiece(to.getX(), to.getY());
    if (pieceToRestore) {
        pieceToRestore->moveTo(oldPosition);
        fromSurface->addPiece(std::move(pieceToRestore), from.getX(), from.getY());
    }

    if (capturedHolder) {
        toSurface->addPiece(std::move(capturedHolder), to.getX(), to.getY());
    }

    return !isKingInCheck;
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

void GameManager::showHint(GameVerdict verdict) const {
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
            break;
    }
}

ChessPiece* GameManager::getCapturedFigure() const {
    return capturedFigure.get();
}

