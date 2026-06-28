#include <iostream>
#include <string>
#include <sstream>
#include "../include/game/GameManager.hpp"
#include "../include/game/Player.hpp"
#include "../include/board/surfaces/CylindricalSurface.hpp"
#include "../include/board/surfaces/SphericalSurface.hpp"
#include "../include/board/GameBoard.hpp"
#include "../include/pieces/ChessPiece.hpp"
#include "../include/common/Enums.hpp"

void printBoard(GameBoard* board, int surfaceId) {
    Surface* surface = board->getSurface(surfaceId);
    if (!surface) {
        std::cout << "Surface " << surfaceId << " not found!" << std::endl;
        return;
    }

    std::cout << "\n=== Surface " << surfaceId;
    if (surfaceId == 0) std::cout << " (Cylindrical) ";
    else if (surfaceId == 1) std::cout << " (Spherical) ";
    std::cout << "===" << std::endl;

    std::cout << "  ";
    for (int x = 0; x < surface->getWidth(); ++x) {
        std::cout << " " << x << " ";
    }
    std::cout << std::endl;

    for (int y = surface->getHeight() - 1; y >= 0; --y) {
        std::cout << y << " ";
        for (int x = 0; x < surface->getWidth(); ++x) {
            ChessPiece* piece = surface->getPieceAt(x, y);
            Cell* cell = surface->getCell(x, y);

            if (piece) {
                char symbol = '.';
                switch (piece->getType()) {
                    case PieceType::KING:   symbol = 'K'; break;
                    case PieceType::QUEEN:  symbol = 'Q'; break;
                    case PieceType::ROOK:   symbol = 'R'; break;
                    case PieceType::BISHOP: symbol = 'B'; break;
                    case PieceType::KNIGHT: symbol = 'N'; break;
                    case PieceType::PAWN:   symbol = 'P'; break;
                }
                if (piece->getColor() == PieceColor::BLACK) {
                    symbol = tolower(symbol);
                }
                std::cout << "[" << symbol << "]";
            } else if (cell && cell->hasPortal()) {
                std::cout << " @ ";
            } else {
                std::cout << " . ";
            }
        }
        std::cout << " " << y << std::endl;
    }

    std::cout << "  ";
    for (int x = 0; x < surface->getWidth(); ++x) {
        std::cout << " " << x << " ";
    }
    std::cout << "\n" << std::endl;
}

void printHelp() {
    std::cout << "\n=== Commands ===" << std::endl;
    std::cout << "  move <surf> <x1> <y1> <surf2> <x2> <y2>  - Make a move" << std::endl;
    std::cout << "  show [surface_id]                        - Show board(s)" << std::endl;
    std::cout << "  save <filename>                          - Save game" << std::endl;
    std::cout << "  load <filename>                          - Load game" << std::endl;
    std::cout << "  portals                                  - Show portal list" << std::endl;
    std::cout << "  help                                     - Show this help" << std::endl;
    std::cout << "  quit                                     - Exit game" << std::endl;
    std::cout << std::endl;
}

std::string getColorName(PieceColor color) {
    return (color == PieceColor::WHITE) ? "White" : "Black";
}

std::string getVerdictName(GameVerdict verdict) {
    switch (verdict) {
        case GameVerdict::CHECK: return "CHECK!";
        case GameVerdict::CHECKMATE: return "CHECKMATE!";
        case GameVerdict::STALEMATE: return "STALEMATE!";
        default: return "";
    }
}

void showPortals(GameBoard* board) {
    std::cout << "\n=== Portals ===" << std::endl;
    bool foundAny = false;

    for (int surfId = 0; surfId < 2; surfId++) {
        Surface* surface = board->getSurface(surfId);
        if (!surface) continue;

        for (int x = 0; x < surface->getWidth(); x++) {
            for (int y = 0; y < surface->getHeight(); y++) {
                Cell* cell = surface->getCell(x, y);
                if (cell && cell->hasPortal()) {
                    PortalLink* portal = cell->getPortalLink();
                    if (portal && portal->isActive()) {
                        auto dest = portal->getDestination();
                        std::cout << "  Surface " << surfId << " (" << x << "," << y << ") -> "
                                  << "Surface " << dest.getSurfaceId() << " ("
                                  << dest.getX() << "," << dest.getY() << ")" << std::endl;
                        foundAny = true;
                    }
                }
            }
        }
    }

    if (!foundAny) {
        std::cout << "  No portals found." << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  Non-Euclidean Chess" << std::endl;
    std::cout << "  Cylinder + Sphere with Portals" << std::endl;
    std::cout << "========================================" << std::endl;

    Player player1("White", PieceColor::WHITE);
    Player player2("Black", PieceColor::BLACK);

    GameManager manager;

    auto cylinder = std::make_unique<CylindricalSurface>(0, 8);
    cylinder->initializeBoard();
    cylinder->setupInitialPosition();

    auto sphere = std::make_unique<SphericalSurface>(1);
    sphere->initializeBoard();

    cylinder->addPortal(4, 4, 1, 4, 4);
    sphere->addPortal(4, 4, 0, 4, 4);

    cylinder->addPortal(3, 5, 1, 3, 3);
    sphere->addPortal(3, 3, 0, 3, 5);

    manager.gameBoard.addSurface(std::move(cylinder));
    manager.gameBoard.addSurface(std::move(sphere));

    manager.startNewGame(&player1, &player2);

    std::cout << "\nInitial setup complete!" << std::endl;
    std::cout << "Type 'help' for commands.\n" << std::endl;

    printBoard(&manager.gameBoard, 0);
    printBoard(&manager.gameBoard, 1);

    bool gameRunning = true;
    int moveCount = 0;
    PieceColor currentColor = PieceColor::WHITE;

    while (gameRunning) {
        std::cout << "Move #" << (moveCount + 1) << " - " << getColorName(currentColor) << "'s turn" << std::endl;
        std::cout << "> ";

        std::string input;
        std::getline(std::cin, input);

        if (input.empty()) continue;

        std::istringstream iss(input);
        std::string command;
        iss >> command;

        if (command == "quit" || command == "exit" || command == "q") {
            std::cout << "\nExiting game. Thanks for playing!" << std::endl;
            gameRunning = false;
            break;
        }

        if (command == "help" || command == "h") {
            printHelp();
            continue;
        }

        if (command == "show" || command == "board") {
            int surfId = -1;
            iss >> surfId;

            if (surfId >= 0 && surfId < 2) {
                printBoard(&manager.gameBoard, surfId);
            } else {
                printBoard(&manager.gameBoard, 0);
                printBoard(&manager.gameBoard, 1);
            }
            continue;
        }

        if (command == "portals") {
            showPortals(&manager.gameBoard);
            continue;
        }

        if (command == "save") {
            std::string filename;
            iss >> filename;
            if (filename.empty()) filename = "savegame.txt";

            manager.saveGame(filename);
            std::cout << "Game saved to: " << filename << std::endl;
            continue;
        }

        if (command == "load") {
            std::string filename;
            iss >> filename;
            if (filename.empty()) filename = "savegame.txt";

            manager.loadGame(filename);
            std::cout << "Game loaded from: " << filename << std::endl;
            printBoard(&manager.gameBoard, 0);
            printBoard(&manager.gameBoard, 1);
            continue;
        }

        if (command == "move" || command == "m") {
            int surf1, x1, y1, surf2, x2, y2;
            if (!(iss >> surf1 >> x1 >> y1 >> surf2 >> x2 >> y2)) {
                std::cout << "Invalid format! Use: move <surf1> <x1> <y1> <surf2> <x2> <y2>" << std::endl;
                continue;
            }

            BoardPosition from(surf1, x1, y1);
            BoardPosition to(surf2, x2, y2);
            Move move(from, to);

            if (manager.validateMove(move)) {
                if (manager.makeMove(move)) {
                    moveCount++;
                    currentColor = (currentColor == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;

                    printBoard(&manager.gameBoard, 0);
                    printBoard(&manager.gameBoard, 1);

                    GameVerdict verdict = manager.calculateVerdict();
                    std::string verdictText = getVerdictName(verdict);
                    if (!verdictText.empty()) {
                        std::cout << ">>> " << verdictText << " <<<" << std::endl;
                        if (verdict == GameVerdict::CHECKMATE) {
                            PieceColor winner = (currentColor == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
                            std::cout << "\n*** " << getColorName(winner) << " WINS! ***\n" << std::endl;
                            gameRunning = false;
                        } else if (verdict == GameVerdict::STALEMATE) {
                            std::cout << "\n*** DRAW! ***\n" << std::endl;
                            gameRunning = false;
                        }
                    }
                } else {
                    std::cout << "Failed to make move!" << std::endl;
                }
            } else {
                std::cout << "Invalid move! Try again." << std::endl;
            }
        } else {
            std::cout << "Unknown command: '" << command << "'. Type 'help' for commands." << std::endl;
        }
    }

    std::cout << "\nGame over. Total moves: " << moveCount << std::endl;
    return 0;
}

