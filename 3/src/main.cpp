#include <iostream>
#include <string>
#include <sstream>
#include "../include/game/GameManager.hpp"
#include "../include/game/Player.hpp"
#include "../include/board/surfaces/CylindricalSurface.hpp"
#include "../include/board/GameBoard.hpp"
#include "../include/pieces/ChessPiece.hpp"
#include "../include/common/Enums.hpp"

void printBoard(GameBoard* board, int surfaceId) {
    Surface* surface = board->getSurface(surfaceId);
    if (!surface) {
        std::cout << "Surface not found!" << std::endl;
        return;
    }

    std::cout << "\n  ";
    for (int x = 0; x < surface->getWidth(); ++x) {
        std::cout << " " << x << " ";
    }
    std::cout << std::endl;

    for (int y = surface->getHeight() - 1; y >= 0; --y) {
        std::cout << y << " ";
        for (int x = 0; x < surface->getWidth(); ++x) {
            ChessPiece* piece = surface->getPieceAt(x, y);
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
    std::cout << "  move <x1> <y1> <x2> <y2>  - Make a move (e.g., move 0 1 0 3)" << std::endl;
    std::cout << "  show                      - Show current board" << std::endl;
    std::cout << "  help                      - Show this help" << std::endl;
    std::cout << "  quit                      - Exit game" << std::endl;
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

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "\nCylindrical board" << std::endl;

    Player player1("White", PieceColor::WHITE);
    Player player2("Black", PieceColor::BLACK);

    GameManager manager;

    auto surface = std::make_unique<CylindricalSurface>(0, 8);
    surface->initializeBoard();
    surface->setupInitialPosition();

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(&player1, &player2);

    printBoard(&manager.gameBoard, 0);

    bool gameRunning = true;
    int moveCount = 0;
    PieceColor currentColor = PieceColor::WHITE;

    while (gameRunning) {
        std::cout << "Move #" << (moveCount + 1) << " - " << getColorName(currentColor) << "'s turn" << std::endl;
        std::cout << "> ";

        std::string input;
        std::getline(std::cin, input);

        if (input.empty()) {
            continue;
        }

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
            printBoard(&manager.gameBoard, 0);
            continue;
        }

        if (command == "move" || command == "m") {
            int x1, y1, x2, y2;
            if (!(iss >> x1 >> y1 >> x2 >> y2)) {
                std::cout << "Invalid format! Use: move <x1> <y1> <x2> <y2>" << std::endl;
                continue;
            }

            BoardPosition from(0, x1, y1);
            BoardPosition to(0, x2, y2);
            Move move(from, to);

            if (manager.validateMove(move)) {
                if (manager.makeMove(move)) {
                    moveCount++;
                    currentColor = (currentColor == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;

                    printBoard(&manager.gameBoard, 0);

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

