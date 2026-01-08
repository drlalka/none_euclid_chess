#include <chrono>
#include <fstream>
#include <vector>
#include <random>
#include <set>
#include <tuple>
#include <iostream>
#include <cmath>
#include "../include/board/GameBoard.hpp"
#include "../include/board/surfaces/StandardSurface.hpp"
#include "../include/pieces/King.hpp"
#include "../include/pieces/Queen.hpp"
#include "../include/pieces/Rook.hpp"
#include "../include/pieces/Bishop.hpp"
#include "../include/pieces/Knight.hpp"
#include "../include/pieces/Pawn.hpp"
#include "../include/utils/ThreadedVerdictCalculator.hpp"

GameBoard* createRandomBoard(int numPieces, int seed) {
    std::mt19937 rng(seed);

    int piecesPerSurface = numPieces / 3;
    int totalCellsPerSurface = static_cast<int>(piecesPerSurface / 0.3);
    totalCellsPerSurface = (totalCellsPerSurface / 2) * 2;
    if (totalCellsPerSurface < 64) totalCellsPerSurface = 64;

    int width = 8;
    while (width * width < totalCellsPerSurface) width += 2;
    int height = totalCellsPerSurface / width;
    if (height < 8) height = 8;
    if (height % 2 != 0) height++;

    GameBoard* board = new GameBoard();

    for (int surfaceId = 0; surfaceId < 3; surfaceId++) {
        auto surface = std::make_unique<StandardSurface>(surfaceId, width, height);
        surface->initializeBoard();

        std::uniform_int_distribution<int> xDist(0, width - 1);
        std::uniform_int_distribution<int> yDist(0, height - 1);
        std::uniform_int_distribution<int> typeDist(0, 5);

        std::set<std::pair<int, int>> occupied;

        int piecesToPlace = (surfaceId < 2) ? piecesPerSurface : (numPieces - 2 * piecesPerSurface);

        if (surfaceId == 0) {
            int x = xDist(rng);
            int y = yDist(rng);
            occupied.insert({x, y});
            surface->addPiece(std::make_unique<King>(PieceColor::WHITE, BoardPosition(surfaceId, x, y)), x, y);
        }

        if (surfaceId == 1) {
            int x, y;
            do {
                x = xDist(rng);
                y = yDist(rng);
            } while (occupied.count({x, y}));
            occupied.insert({x, y});
            surface->addPiece(std::make_unique<King>(PieceColor::BLACK, BoardPosition(surfaceId, x, y)), x, y);
        }

        int startPiece = (surfaceId == 0) ? 1 : (surfaceId == 1 ? 1 : 0);
        for (int i = startPiece; i < piecesToPlace; i++) {
            int x, y;
            do {
                x = xDist(rng);
                y = yDist(rng);
            } while (occupied.count({x, y}));
            occupied.insert({x, y});

            PieceColor color = (i % 2 == 0) ? PieceColor::WHITE : PieceColor::BLACK;
            int type = typeDist(rng);

            std::unique_ptr<ChessPiece> piece;
            switch (type) {
                case 0: piece = std::make_unique<Queen>(color, BoardPosition(surfaceId, x, y)); break;
                case 1: piece = std::make_unique<Rook>(color, BoardPosition(surfaceId, x, y)); break;
                case 2: piece = std::make_unique<Bishop>(color, BoardPosition(surfaceId, x, y)); break;
                case 3: piece = std::make_unique<Knight>(color, BoardPosition(surfaceId, x, y)); break;
                case 4: piece = std::make_unique<Pawn>(color, BoardPosition(surfaceId, x, y)); break;
                case 5: piece = std::make_unique<Queen>(color, BoardPosition(surfaceId, x, y)); break;
            }
            surface->addPiece(std::move(piece), x, y);
        }

        board->addSurface(std::move(surface));
    }

    int totalCells = width * height;
    int numPortals = static_cast<int>(totalCells * 0.03);

    std::uniform_int_distribution<int> surfaceDist(0, 2);
    std::uniform_int_distribution<int> xDist(0, width - 1);
    std::uniform_int_distribution<int> yDist(0, height - 1);

    std::set<std::tuple<int, int, int>> usedPortalCells;
    std::set<std::tuple<int, int, int>> portalDestinations;

    for (int i = 0; i < numPortals * 3; i++) {
        int fromSurface = surfaceDist(rng);
        int toSurface = surfaceDist(rng);
        int fromX = xDist(rng);
        int fromY = yDist(rng);
        int toX = xDist(rng);
        int toY = yDist(rng);

        auto fromKey = std::make_tuple(fromSurface, fromX, fromY);
        auto toKey = std::make_tuple(toSurface, toX, toY);

        if (usedPortalCells.count(fromKey)) continue;
        if (fromKey == toKey) continue;
        if (portalDestinations.count(toKey)) continue;

        Surface* surface = board->getSurface(fromSurface);
        if (surface) {
            Cell* cell = surface->getCell(fromX, fromY);
            if (cell && cell->isEmpty() && !cell->hasPortal()) {
                surface->addPortal(fromX, fromY, toSurface, toX, toY);
                usedPortalCells.insert(fromKey);
                portalDestinations.insert(toKey);
            }
        }
    }

    return board;
}

double benchmarkThreads(int numPieces, int numTests, int numThreads) {
    ThreadedVerdictCalculator calc;
    calc.setNumThreads(numThreads);

    std::vector<GameBoard*> boards;
    for (int i = 0; i < numTests; i++) {
        boards.push_back(createRandomBoard(numPieces, 420 + i));
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (auto* board : boards) {
        calc.calculateGameState(board, PieceColor::WHITE);
    }
    auto end = std::chrono::high_resolution_clock::now();

    for (auto* board : boards) {
        delete board;
    }

    double totalMs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
    return totalMs / numTests;
}

int main() {
    std::ofstream thread1("thread1.csv");
    std::ofstream thread2("thread2.csv");
    std::ofstream thread4("thread4.csv");
    std::ofstream thread8("thread8.csv");

    thread1 << "NumPieces,Time_ms\n";
    thread2 << "NumPieces,Time_ms\n";
    thread4 << "NumPieces,Time_ms\n";
    thread8 << "NumPieces,Time_ms\n";

    for (int n = 100; n <= 5000; n += 100) {
        std::cout << "Testing " << n << " pieces..." << std::flush;

        double time8 = benchmarkThreads(n, 10, 8);
        double time1 = benchmarkThreads(n, 10, 1);
        double time2 = benchmarkThreads(n, 10, 2);
        double time4 = benchmarkThreads(n, 10, 4);

        thread1 << n << "," << time1 << "\n";
        thread2 << n << "," << time2 << "\n";
        thread4 << n << "," << time4 << "\n";
        thread8 << n << "," << time8 << "\n";

        thread1.flush();
        thread2.flush();
        thread4.flush();
        thread8.flush();

        std::cout << " Done.\n";
    }

    thread1.close();
    thread2.close();
    thread4.close();
    thread8.close();

    return 0;
}

