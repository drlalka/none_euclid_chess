#include "../../include/game/GameManager.hpp"
#include "../../include/common/Move.hpp"
#include "../../include/pieces/ChessPiece.hpp"
#include "../../include/pieces/King.hpp"
#include "../../include/pieces/Queen.hpp"
#include "../../include/pieces/Rook.hpp"
#include "../../include/pieces/Bishop.hpp"
#include "../../include/pieces/Knight.hpp"
#include "../../include/pieces/Pawn.hpp"
#include "../../include/board/surfaces/RectangularSurface.hpp"
#include "../../include/board/surfaces/CylindricalSurface.hpp"
#include "../../include/board/surfaces/SphericalSurface.hpp"
#include "../../include/board/surfaces/StandardSurface.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

GameManager::GameManager()
    : currentState(GameState::NO_GAME),
      currentVerdict(GameVerdict::NONE),
      currentPlayerId(0) {
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

    std::unique_ptr<ChessPiece> capturedHolder;
    Surface* toSurfaceForCapture = gameBoard.getSurface(to.getSurfaceId());
    if (toSurfaceForCapture && gameBoard.getPieceAt(to)) {
        capturedHolder = toSurfaceForCapture->removePiece(to.getX(), to.getY());
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

    gameHistory.clear();
    chessClock.restartTimer();
    chessClock.startTimer(PieceColor::WHITE);
}

void GameManager::saveGame(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for saving: " << filename << std::endl;
        return;
    }

    file << "# Non-Euclidean Chess Save File\n\n";

    file << "[SURFACES]\n";
    for (int id = 0; id < 10; id++) {
        Surface* surface = gameBoard.getSurface(id);
        if (!surface) continue;

        file << "surface " << id << " ";
        if (dynamic_cast<RectangularSurface*>(surface)) {
            file << "rectangular 8\n";
        } else if (dynamic_cast<CylindricalSurface*>(surface)) {
            file << "cylindrical 8\n";
        } else if (dynamic_cast<SphericalSurface*>(surface)) {
            file << "spherical\n";
        } else if (dynamic_cast<StandardSurface*>(surface)) {
            file << "standard 8 8\n";
        }
    }
    file << "\n";

    file << "[PIECES]\n";
    auto pieces = gameBoard.getAllPieces();
    for (const auto* piece : pieces) {
        const auto& pos = piece->getPosition();
        char typeChar = 'P';
        switch (piece->getType()) {
            case PieceType::KING: typeChar = 'K'; break;
            case PieceType::QUEEN: typeChar = 'Q'; break;
            case PieceType::ROOK: typeChar = 'R'; break;
            case PieceType::BISHOP: typeChar = 'B'; break;
            case PieceType::KNIGHT: typeChar = 'N'; break;
            case PieceType::PAWN: typeChar = 'P'; break;
        }
        char colorChar = (piece->getColor() == PieceColor::WHITE) ? 'W' : 'B';
        file << typeChar << colorChar << " " << pos.getSurfaceId() << " "
             << pos.getX() << " " << pos.getY() << "\n";
    }
    file << "\n";

    file << "[PORTALS]\n";
    for (int id = 0; id < 10; id++) {
        Surface* surface = gameBoard.getSurface(id);
        if (!surface) continue;
        for (int x = 0; x < 100; x++) {
            for (int y = 0; y < 100; y++) {
                Cell* cell = surface->getCell(x, y);
                if (!cell || !cell->hasPortal()) continue;
                PortalLink* portal = cell->getPortalLink();
                if (!portal || !portal->isActive()) continue;
                auto dest = portal->getDestination();
                file << id << " " << x << " " << y << " -> "
                     << dest.getSurfaceId() << " " << dest.getX() << " " << dest.getY() << "\n";
            }
        }
    }
    file << "\n";

    file << "[MOVES]\n";
    const auto& moves = gameHistory.getMoves();
    for (const auto& move : moves) {
        const auto& from = move.getFrom();
        const auto& to = move.getTo();
        file << from.getSurfaceId() << ":" << from.getX() << "," << from.getY() << " "
             << to.getSurfaceId() << ":" << to.getX() << "," << to.getY() << " "
             << (move.isPortalUsed() ? "1" : "0") << "\n";
    }
    file << "\n";

    file << "[GAME_STATE]\n";
    file << "state " << static_cast<int>(currentState) << "\n";
    file << "player " << currentPlayerId << "\n";

    file.close();
}

void GameManager::loadGame(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for loading: " << filename << std::endl;
        return;
    }

    std::string line, section;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        if (line[0] == '[') {
            section = line;
            continue;
        }

        if (section == "[SURFACES]") {
            std::istringstream iss(line);
            std::string cmd;
            int id;
            std::string type;
            iss >> cmd >> id >> type;

            std::unique_ptr<Surface> surface;
            if (type == "rectangular") {
                int height;
                iss >> height;
                surface = std::make_unique<RectangularSurface>(id, height);
            } else if (type == "cylindrical") {
                int height;
                iss >> height;
                surface = std::make_unique<CylindricalSurface>(id, height);
            } else if (type == "spherical") {
                surface = std::make_unique<SphericalSurface>(id);
            } else if (type == "standard") {
                int width, height;
                iss >> width >> height;
                surface = std::make_unique<StandardSurface>(id, width, height);
            }

            if (surface) {
                surface->initializeBoard();
                gameBoard.addSurface(std::move(surface));
            }
        } else if (section == "[PIECES]") {
            std::istringstream iss(line);
            std::string pieceCode;
            int surfaceId, x, y;
            iss >> pieceCode >> surfaceId >> x >> y;

            if (pieceCode.length() < 2) continue;

            char typeChar = pieceCode[0];
            char colorChar = pieceCode[1];
            PieceColor color = (colorChar == 'W') ? PieceColor::WHITE : PieceColor::BLACK;
            BoardPosition pos(surfaceId, x, y);

            std::unique_ptr<ChessPiece> piece;
            switch (typeChar) {
                case 'K': piece = std::make_unique<King>(color, pos); break;
                case 'Q': piece = std::make_unique<Queen>(color, pos); break;
                case 'R': piece = std::make_unique<Rook>(color, pos); break;
                case 'B': piece = std::make_unique<Bishop>(color, pos); break;
                case 'N': piece = std::make_unique<Knight>(color, pos); break;
                case 'P': piece = std::make_unique<Pawn>(color, pos); break;
            }

            if (piece) {
                Surface* surface = gameBoard.getSurface(surfaceId);
                if (surface) {
                    surface->addPiece(std::move(piece), x, y);
                }
            }
        } else if (section == "[PORTALS]") {
            std::istringstream iss(line);
            int fromSurf, fromX, fromY, toSurf, toX, toY;
            std::string arrow;
            iss >> fromSurf >> fromX >> fromY >> arrow >> toSurf >> toX >> toY;

            Surface* surface = gameBoard.getSurface(fromSurf);
            if (surface) {
                surface->addPortal(fromX, fromY, toSurf, toX, toY);
            }
        } else if (section == "[GAME_STATE]") {
            std::istringstream iss(line);
            std::string key;
            iss >> key;
            if (key == "state") {
                int stateInt;
                iss >> stateInt;
                currentState = static_cast<GameState>(stateInt);
            } else if (key == "player") {
                iss >> currentPlayerId;
            }
        }
    }

    file.close();
}

void GameManager::showHint(GameVerdict verdict) const {
    switch (verdict) {
        case GameVerdict::CHECK:
            std::cout << "check" << std::endl;
            break;
        case GameVerdict::CHECKMATE:
            std::cout << "end" << std::endl;
            break;
        case GameVerdict::STALEMATE:
            std::cout << "lol" << std::endl;
            break;
        case GameVerdict::NONE:
            break;
    }
}


