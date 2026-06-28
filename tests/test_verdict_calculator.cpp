#include <gtest/gtest.h>
#include "../include/utils/ThreadedVerdictCalculator.hpp"
#include "../include/game/GameManager.hpp"
#include "../include/game/Player.hpp"
#include "../include/board/surfaces/RectangularSurface.hpp"
#include "../include/pieces/King.hpp"
#include "../include/pieces/Queen.hpp"
#include "../include/pieces/Rook.hpp"
#include "../include/pieces/Pawn.hpp"

class VerdictCalculatorTest : public ::testing::Test {
protected:
    void SetUp() override {
        calculator = new ThreadedVerdictCalculator();
    }

    void TearDown() override {
        delete calculator;
    }

    ThreadedVerdictCalculator* calculator;
};

TEST_F(VerdictCalculatorTest, NoCheckNormalPosition) {
    GameManager manager;

    auto surface = std::make_unique<RectangularSurface>(0, 8);
    surface->initializeBoard();
    surface->setupInitialPosition();

    manager.gameBoard.addSurface(std::move(surface));

    GameVerdict verdict = calculator->calculateGameState(&manager.gameBoard, PieceColor::WHITE);

    EXPECT_NE(verdict, GameVerdict::CHECK);
    EXPECT_NE(verdict, GameVerdict::CHECKMATE);
}

TEST_F(VerdictCalculatorTest, CheckDetection) {
    GameManager manager;

    auto surface = std::make_unique<RectangularSurface>(0, 8);
    surface->initializeBoard();

    auto whiteKing = std::make_unique<King>(PieceColor::WHITE, BoardPosition(0, 4, 0));
    auto blackQueen = std::make_unique<Queen>(PieceColor::BLACK, BoardPosition(0, 4, 7));

    surface->addPiece(std::move(whiteKing), 4, 0);
    surface->addPiece(std::move(blackQueen), 4, 7);

    manager.gameBoard.addSurface(std::move(surface));

    GameVerdict verdict = calculator->calculateGameState(&manager.gameBoard, PieceColor::WHITE);

    EXPECT_EQ(verdict, GameVerdict::CHECK);
}

TEST_F(VerdictCalculatorTest, DISABLED_CheckmateDetection) {
    GameManager manager;

    auto surface = std::make_unique<RectangularSurface>(0, 8);
    surface->initializeBoard();

    auto whiteKing = std::make_unique<King>(PieceColor::WHITE, BoardPosition(0, 0, 0));
    auto blackQueen = std::make_unique<Queen>(PieceColor::BLACK, BoardPosition(0, 1, 2));
    auto blackRook = std::make_unique<Rook>(PieceColor::BLACK, BoardPosition(0, 2, 0));

    surface->addPiece(std::move(whiteKing), 0, 0);
    surface->addPiece(std::move(blackQueen), 1, 2);
    surface->addPiece(std::move(blackRook), 2, 0);

    manager.gameBoard.addSurface(std::move(surface));

    GameVerdict verdict = calculator->calculateGameState(&manager.gameBoard, PieceColor::WHITE);

    EXPECT_EQ(verdict, GameVerdict::CHECKMATE);
}

TEST_F(VerdictCalculatorTest, DISABLED_StalemateDetection) {
    GameManager manager;

    auto surface = std::make_unique<RectangularSurface>(0, 8);
    surface->initializeBoard();

    auto whiteKing = std::make_unique<King>(PieceColor::WHITE, BoardPosition(0, 0, 0));
    auto blackQueen = std::make_unique<Queen>(PieceColor::BLACK, BoardPosition(0, 2, 1));
    auto blackKing = std::make_unique<King>(PieceColor::BLACK, BoardPosition(0, 2, 2));

    surface->addPiece(std::move(whiteKing), 0, 0);
    surface->addPiece(std::move(blackQueen), 2, 1);
    surface->addPiece(std::move(blackKing), 2, 2);

    manager.gameBoard.addSurface(std::move(surface));

    GameVerdict verdict = calculator->calculateGameState(&manager.gameBoard, PieceColor::WHITE);

    EXPECT_EQ(verdict, GameVerdict::STALEMATE);
}

TEST_F(VerdictCalculatorTest, NoVerdictWithMoves) {
    GameManager manager;

    auto surface = std::make_unique<RectangularSurface>(0, 8);
    surface->initializeBoard();

    auto whiteKing = std::make_unique<King>(PieceColor::WHITE, BoardPosition(0, 4, 4));
    auto whitePawn = std::make_unique<Pawn>(PieceColor::WHITE, BoardPosition(0, 4, 5));
    auto blackKing = std::make_unique<King>(PieceColor::BLACK, BoardPosition(0, 4, 7));

    surface->addPiece(std::move(whiteKing), 4, 4);
    surface->addPiece(std::move(whitePawn), 4, 5);
    surface->addPiece(std::move(blackKing), 4, 7);

    manager.gameBoard.addSurface(std::move(surface));

    GameVerdict verdict = calculator->calculateGameState(&manager.gameBoard, PieceColor::WHITE);

    EXPECT_EQ(verdict, GameVerdict::NONE);
}

TEST_F(VerdictCalculatorTest, MultithreadedPerformance) {
    GameManager manager;

    auto surface = std::make_unique<RectangularSurface>(0, 8);
    surface->initializeBoard();
    surface->setupInitialPosition();

    manager.gameBoard.addSurface(std::move(surface));

    auto start = std::chrono::high_resolution_clock::now();
    GameVerdict verdict = calculator->calculateGameState(&manager.gameBoard, PieceColor::WHITE);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    EXPECT_LT(duration.count(), 1000);
    EXPECT_NE(verdict, GameVerdict::CHECKMATE);
}

TEST_F(VerdictCalculatorTest, BlackKingInCheck) {
    GameManager manager;

    auto surface = std::make_unique<RectangularSurface>(0, 8);
    surface->initializeBoard();

    auto blackKing = std::make_unique<King>(PieceColor::BLACK, BoardPosition(0, 4, 7));
    auto whiteQueen = std::make_unique<Queen>(PieceColor::WHITE, BoardPosition(0, 4, 0));

    surface->addPiece(std::move(blackKing), 4, 7);
    surface->addPiece(std::move(whiteQueen), 4, 0);

    manager.gameBoard.addSurface(std::move(surface));

    GameVerdict verdict = calculator->calculateGameState(&manager.gameBoard, PieceColor::BLACK);

    EXPECT_EQ(verdict, GameVerdict::CHECK);
}

