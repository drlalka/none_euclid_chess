#include <gtest/gtest.h>
#include "../include/game/GameManager.hpp"
#include "../include/game/Player.hpp"
#include "../include/board/surfaces/RectangularSurface.hpp"
#include "../include/pieces/Rook.hpp"
#include "../include/pieces/Pawn.hpp"
#include "../include/pieces/Knight.hpp"

class MoveValidationTest : public ::testing::Test {
protected:
    void SetUp() override {
        player1 = new Player("White", PieceColor::WHITE);
        player2 = new Player("Black", PieceColor::BLACK);
    }

    void TearDown() override {
        delete player1;
        delete player2;
    }

    Player* player1;
    Player* player2;
};

TEST_F(MoveValidationTest, ValidatePieceExists) {
    GameManager manager;

    auto surface = std::make_unique<RectangularSurface>(0, 8);
    surface->initializeBoard();

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    BoardPosition emptyFrom(0, 3, 3);
    BoardPosition emptyTo(0, 3, 4);
    Move invalidMove(emptyFrom, emptyTo);

    EXPECT_FALSE(manager.validateMove(invalidMove));
}

TEST_F(MoveValidationTest, ValidateCorrectPlayer) {
    GameManager manager;

    auto surface = std::make_unique<RectangularSurface>(0, 8);
    surface->initializeBoard();

    auto whitePawn = std::make_unique<Pawn>(PieceColor::WHITE, BoardPosition(0, 0, 1));
    auto blackPawn = std::make_unique<Pawn>(PieceColor::BLACK, BoardPosition(0, 0, 6));

    surface->addPiece(std::move(whitePawn), 0, 1);
    surface->addPiece(std::move(blackPawn), 0, 6);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    BoardPosition blackFrom(0, 0, 6);
    BoardPosition blackTo(0, 0, 5);
    Move blackMove(blackFrom, blackTo);

    EXPECT_FALSE(manager.validateMove(blackMove));
}

TEST_F(MoveValidationTest, ValidateRookStraightMove) {
    GameManager manager;

    auto surface = std::make_unique<RectangularSurface>(0, 8);
    surface->initializeBoard();

    auto whiteRook = std::make_unique<Rook>(PieceColor::WHITE, BoardPosition(0, 0, 0));
    surface->addPiece(std::move(whiteRook), 0, 0);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    BoardPosition from(0, 0, 0);
    BoardPosition to(0, 0, 4);
    Move validMove(from, to);

    EXPECT_TRUE(manager.validateMove(validMove));
}

TEST_F(MoveValidationTest, ValidateRookInvalidDiagonal) {
    GameManager manager;

    auto surface = std::make_unique<RectangularSurface>(0, 8);
    surface->initializeBoard();

    auto whiteRook = std::make_unique<Rook>(PieceColor::WHITE, BoardPosition(0, 0, 0));
    surface->addPiece(std::move(whiteRook), 0, 0);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    BoardPosition from(0, 0, 0);
    BoardPosition to(0, 3, 3);
    Move invalidMove(from, to);

    EXPECT_FALSE(manager.validateMove(invalidMove));
}

TEST_F(MoveValidationTest, ValidatePawnForwardMove) {
    GameManager manager;

    auto surface = std::make_unique<RectangularSurface>(0, 8);
    surface->initializeBoard();

    auto whitePawn = std::make_unique<Pawn>(PieceColor::WHITE, BoardPosition(0, 0, 1));
    surface->addPiece(std::move(whitePawn), 0, 1);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    BoardPosition from(0, 0, 1);
    BoardPosition to(0, 0, 2);
    Move validMove(from, to);

    EXPECT_TRUE(manager.validateMove(validMove));
}

TEST_F(MoveValidationTest, ValidatePawnDoubleMove) {
    GameManager manager;

    auto surface = std::make_unique<RectangularSurface>(0, 8);
    surface->initializeBoard();

    auto whitePawn = std::make_unique<Pawn>(PieceColor::WHITE, BoardPosition(0, 0, 1));
    surface->addPiece(std::move(whitePawn), 0, 1);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    BoardPosition from(0, 0, 1);
    BoardPosition to(0, 0, 3);
    Move validMove(from, to);

    EXPECT_TRUE(manager.validateMove(validMove));
}

TEST_F(MoveValidationTest, ValidateKnightLMove) {
    GameManager manager;

    auto surface = std::make_unique<RectangularSurface>(0, 8);
    surface->initializeBoard();

    auto whiteKnight = std::make_unique<Knight>(PieceColor::WHITE, BoardPosition(0, 3, 3));
    surface->addPiece(std::move(whiteKnight), 3, 3);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    BoardPosition from(0, 3, 3);
    BoardPosition to(0, 5, 4);
    Move validMove(from, to);

    EXPECT_TRUE(manager.validateMove(validMove));
}

TEST_F(MoveValidationTest, ValidateSamePositionMove) {
    GameManager manager;

    auto surface = std::make_unique<RectangularSurface>(0, 8);
    surface->initializeBoard();

    auto whiteRook = std::make_unique<Rook>(PieceColor::WHITE, BoardPosition(0, 0, 0));
    surface->addPiece(std::move(whiteRook), 0, 0);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    BoardPosition from(0, 0, 0);
    BoardPosition to(0, 0, 0);
    Move invalidMove(from, to);

    EXPECT_FALSE(manager.validateMove(invalidMove));
}

TEST_F(MoveValidationTest, ValidateBlockedPath) {
    GameManager manager;

    auto surface = std::make_unique<RectangularSurface>(0, 8);
    surface->initializeBoard();

    auto whiteRook = std::make_unique<Rook>(PieceColor::WHITE, BoardPosition(0, 0, 0));
    auto whitePawn = std::make_unique<Pawn>(PieceColor::WHITE, BoardPosition(0, 0, 2));

    surface->addPiece(std::move(whiteRook), 0, 0);
    surface->addPiece(std::move(whitePawn), 0, 2);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    BoardPosition from(0, 0, 0);
    BoardPosition to(0, 0, 5);
    Move invalidMove(from, to);

    EXPECT_FALSE(manager.validateMove(invalidMove));
}

