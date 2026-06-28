#include <gtest/gtest.h>
#include "../include/game/GameManager.hpp"
#include "../include/game/Player.hpp"
#include "../include/board/surfaces/RectangularSurface.hpp"
#include "../include/pieces/King.hpp"
#include "../include/pieces/Queen.hpp"
#include "../include/pieces/Rook.hpp"
#include "../include/pieces/Bishop.hpp"
#include "../include/pieces/Pawn.hpp"

class HypotheticalCheckTest : public ::testing::Test {
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

TEST_F(HypotheticalCheckTest, CannotMoveIntoCheck) {
    GameManager manager;

    auto surface = std::make_unique<RectangularSurface>(0, 8);
    surface->initializeBoard();

    auto whiteKing = std::make_unique<King>(PieceColor::WHITE, BoardPosition(0, 4, 0));
    auto blackQueen = std::make_unique<Queen>(PieceColor::BLACK, BoardPosition(0, 4, 7));

    surface->addPiece(std::move(whiteKing), 4, 0);
    surface->addPiece(std::move(blackQueen), 4, 7);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    Move invalidMove(BoardPosition(0, 4, 0), BoardPosition(0, 4, 1));

    EXPECT_FALSE(manager.validateMove(invalidMove));
}

TEST_F(HypotheticalCheckTest, CannotExposeKingToCheck) {
    GameManager manager;

    auto surface = std::make_unique<RectangularSurface>(0, 8);
    surface->initializeBoard();

    auto whiteKing = std::make_unique<King>(PieceColor::WHITE, BoardPosition(0, 4, 0));
    auto whitePawn = std::make_unique<Pawn>(PieceColor::WHITE, BoardPosition(0, 4, 1));
    auto blackRook = std::make_unique<Rook>(PieceColor::BLACK, BoardPosition(0, 4, 7));

    surface->addPiece(std::move(whiteKing), 4, 0);
    surface->addPiece(std::move(whitePawn), 4, 1);
    surface->addPiece(std::move(blackRook), 4, 7);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    Move invalidMove(BoardPosition(0, 4, 1), BoardPosition(0, 5, 1));

    EXPECT_FALSE(manager.validateMove(invalidMove));
}

TEST_F(HypotheticalCheckTest, CanBlockCheck) {
    GameManager manager;

    auto surface = std::make_unique<RectangularSurface>(0, 8);
    surface->initializeBoard();

    auto whiteKing = std::make_unique<King>(PieceColor::WHITE, BoardPosition(0, 4, 0));
    auto whiteBishop = std::make_unique<Bishop>(PieceColor::WHITE, BoardPosition(0, 2, 2));
    auto blackQueen = std::make_unique<Queen>(PieceColor::BLACK, BoardPosition(0, 4, 7));

    surface->addPiece(std::move(whiteKing), 4, 0);
    surface->addPiece(std::move(whiteBishop), 2, 2);
    surface->addPiece(std::move(blackQueen), 4, 7);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    Move blockingMove(BoardPosition(0, 2, 2), BoardPosition(0, 4, 4));

    EXPECT_TRUE(manager.validateMove(blockingMove));
}

TEST_F(HypotheticalCheckTest, CanCaptureAttacker) {
    GameManager manager;

    auto surface = std::make_unique<RectangularSurface>(0, 8);
    surface->initializeBoard();

    auto whiteKing = std::make_unique<King>(PieceColor::WHITE, BoardPosition(0, 4, 0));
    auto whiteRook = std::make_unique<Rook>(PieceColor::WHITE, BoardPosition(0, 4, 2));
    auto blackQueen = std::make_unique<Queen>(PieceColor::BLACK, BoardPosition(0, 4, 7));

    surface->addPiece(std::move(whiteKing), 4, 0);
    surface->addPiece(std::move(whiteRook), 4, 2);
    surface->addPiece(std::move(blackQueen), 4, 7);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    Move captureMove(BoardPosition(0, 4, 2), BoardPosition(0, 4, 7));

    EXPECT_TRUE(manager.validateMove(captureMove));
}

TEST_F(HypotheticalCheckTest, KingCanEscapeCheck) {
    GameManager manager;

    auto surface = std::make_unique<RectangularSurface>(0, 8);
    surface->initializeBoard();

    auto whiteKing = std::make_unique<King>(PieceColor::WHITE, BoardPosition(0, 4, 0));
    auto blackQueen = std::make_unique<Queen>(PieceColor::BLACK, BoardPosition(0, 4, 7));

    surface->addPiece(std::move(whiteKing), 4, 0);
    surface->addPiece(std::move(blackQueen), 4, 7);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    Move escapeMove(BoardPosition(0, 4, 0), BoardPosition(0, 3, 0));

    EXPECT_TRUE(manager.validateMove(escapeMove));
}

TEST_F(HypotheticalCheckTest, CannotMoveKingIntoCheckFromBishop) {
    GameManager manager;

    auto surface = std::make_unique<RectangularSurface>(0, 8);
    surface->initializeBoard();

    auto whiteKing = std::make_unique<King>(PieceColor::WHITE, BoardPosition(0, 4, 4));
    auto blackBishop = std::make_unique<Bishop>(PieceColor::BLACK, BoardPosition(0, 0, 0));

    surface->addPiece(std::move(whiteKing), 4, 4);
    surface->addPiece(std::move(blackBishop), 0, 0);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    Move invalidMove(BoardPosition(0, 4, 4), BoardPosition(0, 3, 3));

    EXPECT_FALSE(manager.validateMove(invalidMove));
}

TEST_F(HypotheticalCheckTest, PinnedPieceCannotMove) {
    GameManager manager;

    auto surface = std::make_unique<RectangularSurface>(0, 8);
    surface->initializeBoard();

    auto whiteKing = std::make_unique<King>(PieceColor::WHITE, BoardPosition(0, 4, 0));
    auto whiteRook = std::make_unique<Rook>(PieceColor::WHITE, BoardPosition(0, 4, 3));
    auto blackRook = std::make_unique<Rook>(PieceColor::BLACK, BoardPosition(0, 4, 7));

    surface->addPiece(std::move(whiteKing), 4, 0);
    surface->addPiece(std::move(whiteRook), 4, 3);
    surface->addPiece(std::move(blackRook), 4, 7);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    Move invalidMove(BoardPosition(0, 4, 3), BoardPosition(0, 5, 3));

    EXPECT_FALSE(manager.validateMove(invalidMove));
}

TEST_F(HypotheticalCheckTest, PinnedPieceCanMoveAlongPinLine) {
    GameManager manager;

    auto surface = std::make_unique<RectangularSurface>(0, 8);
    surface->initializeBoard();

    auto whiteKing = std::make_unique<King>(PieceColor::WHITE, BoardPosition(0, 4, 0));
    auto whiteRook = std::make_unique<Rook>(PieceColor::WHITE, BoardPosition(0, 4, 3));
    auto blackRook = std::make_unique<Rook>(PieceColor::BLACK, BoardPosition(0, 4, 7));

    surface->addPiece(std::move(whiteKing), 4, 0);
    surface->addPiece(std::move(whiteRook), 4, 3);
    surface->addPiece(std::move(blackRook), 4, 7);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    Move validMove(BoardPosition(0, 4, 3), BoardPosition(0, 4, 5));

    EXPECT_TRUE(manager.validateMove(validMove));
}

TEST_F(HypotheticalCheckTest, CannotLeaveKingInCheck) {
    GameManager manager;

    auto surface = std::make_unique<RectangularSurface>(0, 8);
    surface->initializeBoard();

    auto whiteKing = std::make_unique<King>(PieceColor::WHITE, BoardPosition(0, 4, 0));
    auto whitePawn = std::make_unique<Pawn>(PieceColor::WHITE, BoardPosition(0, 3, 1));
    auto blackQueen = std::make_unique<Queen>(PieceColor::BLACK, BoardPosition(0, 4, 7));

    surface->addPiece(std::move(whiteKing), 4, 0);
    surface->addPiece(std::move(whitePawn), 3, 1);
    surface->addPiece(std::move(blackQueen), 4, 7);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    Move invalidMove(BoardPosition(0, 3, 1), BoardPosition(0, 3, 2));

    EXPECT_FALSE(manager.validateMove(invalidMove));
}

TEST_F(HypotheticalCheckTest, NormalMoveStillWorks) {
    GameManager manager;

    auto surface = std::make_unique<RectangularSurface>(0, 8);
    surface->initializeBoard();

    auto whiteKing = std::make_unique<King>(PieceColor::WHITE, BoardPosition(0, 4, 0));
    auto whitePawn = std::make_unique<Pawn>(PieceColor::WHITE, BoardPosition(0, 2, 1));
    auto blackKing = std::make_unique<King>(PieceColor::BLACK, BoardPosition(0, 4, 7));

    surface->addPiece(std::move(whiteKing), 4, 0);
    surface->addPiece(std::move(whitePawn), 2, 1);
    surface->addPiece(std::move(blackKing), 4, 7);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    Move normalMove(BoardPosition(0, 2, 1), BoardPosition(0, 2, 2));

    EXPECT_TRUE(manager.validateMove(normalMove));
}

