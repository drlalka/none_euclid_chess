#include <gtest/gtest.h>
#include "../include/game/GameManager.hpp"
#include "../include/game/Player.hpp"
#include "../include/board/surfaces/SphericalSurface.hpp"
#include "../include/board/surfaces/CylindricalSurface.hpp"
#include "../include/board/surfaces/RectangularSurface.hpp"
#include "../include/pieces/Queen.hpp"
#include "../include/pieces/Rook.hpp"
#include "../include/pieces/Bishop.hpp"
#include <set>

class NonEuclideanSurfaceTest : public ::testing::Test {
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

TEST_F(NonEuclideanSurfaceTest, QueenOnCylinderWrapsAround) {
    GameManager manager;

    auto surface = std::make_unique<CylindricalSurface>(0, 8);
    surface->initializeBoard();

    auto queen = std::make_unique<Queen>(PieceColor::WHITE, BoardPosition(0, 0, 4));
    surface->addPiece(std::move(queen), 0, 4);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    auto piece = manager.gameBoard.getPieceAt(BoardPosition(0, 0, 4));
    auto moves = piece->getPossibleMoves(&manager.gameBoard);

    std::set<std::pair<int, int>> positions;
    for (const auto& move : moves) {
        if (move.getTo().getY() == 4) {
            positions.insert({move.getTo().getX(), move.getTo().getY()});
        }
    }

    EXPECT_EQ(positions.size(), 7);
    EXPECT_TRUE(positions.count({7, 4}) > 0);
    EXPECT_TRUE(positions.count({1, 4}) > 0);
}

TEST_F(NonEuclideanSurfaceTest, RookOnCylinderFullCircle) {
    GameManager manager;

    auto surface = std::make_unique<CylindricalSurface>(0, 8);
    surface->initializeBoard();

    auto rook = std::make_unique<Rook>(PieceColor::WHITE, BoardPosition(0, 0, 4));
    surface->addPiece(std::move(rook), 0, 4);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    auto piece = manager.gameBoard.getPieceAt(BoardPosition(0, 0, 4));
    auto moves = piece->getPossibleMoves(&manager.gameBoard);

    int horizontalMoves = 0;
    for (const auto& move : moves) {
        if (move.getTo().getY() == 4) {
            horizontalMoves++;
        }
    }

    EXPECT_EQ(horizontalMoves, 7);
}

TEST_F(NonEuclideanSurfaceTest, BishopOnCylinderDiagonal) {
    GameManager manager;

    auto surface = std::make_unique<CylindricalSurface>(0, 8);
    surface->initializeBoard();

    auto bishop = std::make_unique<Bishop>(PieceColor::WHITE, BoardPosition(0, 0, 4));
    surface->addPiece(std::move(bishop), 0, 4);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    auto piece = manager.gameBoard.getPieceAt(BoardPosition(0, 0, 4));
    auto moves = piece->getPossibleMoves(&manager.gameBoard);

    EXPECT_GT(moves.size(), 0);

    bool foundDiagonalMove = false;
    for (const auto& move : moves) {
        if (move.getTo().getX() != 0 && move.getTo().getY() != 4) {
            foundDiagonalMove = true;
            break;
        }
    }
    EXPECT_TRUE(foundDiagonalMove);
}

TEST_F(NonEuclideanSurfaceTest, QueenOnSphereNorthPole) {
    GameManager manager;

    auto surface = std::make_unique<SphericalSurface>(0);
    surface->initializeBoard();

    auto queen = std::make_unique<Queen>(PieceColor::WHITE, BoardPosition(0, 0, 7));
    surface->addPiece(std::move(queen), 0, 7);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    auto piece = manager.gameBoard.getPieceAt(BoardPosition(0, 0, 7));
    auto moves = piece->getPossibleMoves(&manager.gameBoard);

    bool canMoveEast = false;
    bool canMoveSouth = false;

    for (const auto& move : moves) {
        if (move.getTo().getY() == 7 && move.getTo().getX() == 1) {
            canMoveEast = true;
        }
        if (move.getTo().getY() == 6) {
            canMoveSouth = true;
        }
    }

    EXPECT_TRUE(canMoveEast);
    EXPECT_TRUE(canMoveSouth);
}

TEST_F(NonEuclideanSurfaceTest, RookOnSpherePoleRing) {
    GameManager manager;

    auto surface = std::make_unique<SphericalSurface>(0);
    surface->initializeBoard();

    auto rook = std::make_unique<Rook>(PieceColor::WHITE, BoardPosition(0, 0, 7));
    surface->addPiece(std::move(rook), 0, 7);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    auto piece = manager.gameBoard.getPieceAt(BoardPosition(0, 0, 7));
    auto moves = piece->getPossibleMoves(&manager.gameBoard);

    EXPECT_GT(moves.size(), 0);
    EXPECT_LT(moves.size(), 100);

    bool canMoveOnPole = false;
    bool canMoveSouth = false;
    for (const auto& move : moves) {
        if (move.getTo().getY() == 7 && move.getTo().getX() != 0) {
            canMoveOnPole = true;
        }
        if (move.getTo().getY() == 6) {
            canMoveSouth = true;
        }
    }

    EXPECT_TRUE(canMoveOnPole);
    EXPECT_TRUE(canMoveSouth);
}

TEST_F(NonEuclideanSurfaceTest, QueenDoesNotLoopInfinitely) {
    GameManager manager;

    auto surface = std::make_unique<CylindricalSurface>(0, 8);
    surface->initializeBoard();

    auto queen = std::make_unique<Queen>(PieceColor::WHITE, BoardPosition(0, 0, 4));
    surface->addPiece(std::move(queen), 0, 4);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    auto piece = manager.gameBoard.getPieceAt(BoardPosition(0, 0, 4));
    auto moves = piece->getPossibleMoves(&manager.gameBoard);

    EXPECT_LT(moves.size(), 100);
}



TEST_F(NonEuclideanSurfaceTest, QueenCanCaptureEnemyPiece) {
    GameManager manager;

    auto surface = std::make_unique<CylindricalSurface>(0, 8);
    surface->initializeBoard();

    auto queen = std::make_unique<Queen>(PieceColor::WHITE, BoardPosition(0, 0, 4));
    auto enemyPawn = std::make_unique<Rook>(PieceColor::BLACK, BoardPosition(0, 3, 4));

    surface->addPiece(std::move(queen), 0, 4);
    surface->addPiece(std::move(enemyPawn), 3, 4);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    auto piece = manager.gameBoard.getPieceAt(BoardPosition(0, 0, 4));
    auto moves = piece->getPossibleMoves(&manager.gameBoard);

    bool canCaptureEnemy = false;
    for (const auto& move : moves) {
        if (move.getTo().getX() == 3 && move.getTo().getY() == 4) {
            canCaptureEnemy = true;
        }
    }

    EXPECT_TRUE(canCaptureEnemy);
}

TEST_F(NonEuclideanSurfaceTest, RectangularSurfaceNoWraparound) {
    GameManager manager;

    auto surface = std::make_unique<RectangularSurface>(0, 8);
    surface->initializeBoard();

    auto rook = std::make_unique<Rook>(PieceColor::WHITE, BoardPosition(0, 7, 4));
    surface->addPiece(std::move(rook), 7, 4);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    auto piece = manager.gameBoard.getPieceAt(BoardPosition(0, 7, 4));
    auto moves = piece->getPossibleMoves(&manager.gameBoard);

    int horizontalMoves = 0;
    for (const auto& move : moves) {
        if (move.getTo().getY() == 4) {
            horizontalMoves++;
        }
    }

    EXPECT_EQ(horizontalMoves, 7);
}

TEST_F(NonEuclideanSurfaceTest, SphereSouthPoleRing) {
    GameManager manager;

    auto surface = std::make_unique<SphericalSurface>(0);
    surface->initializeBoard();

    auto rook = std::make_unique<Rook>(PieceColor::WHITE, BoardPosition(0, 4, 0));
    surface->addPiece(std::move(rook), 4, 0);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    auto piece = manager.gameBoard.getPieceAt(BoardPosition(0, 4, 0));
    auto moves = piece->getPossibleMoves(&manager.gameBoard);

    EXPECT_GT(moves.size(), 0);
    EXPECT_LT(moves.size(), 100);

    bool canMoveOnPole = false;
    bool canMoveNorth = false;
    for (const auto& move : moves) {
        if (move.getTo().getY() == 0 && move.getTo().getX() != 4) {
            canMoveOnPole = true;
        }
        if (move.getTo().getY() == 1) {
            canMoveNorth = true;
        }
    }

    EXPECT_TRUE(canMoveOnPole);
    EXPECT_TRUE(canMoveNorth);
}

TEST_F(NonEuclideanSurfaceTest, BishopCylinderWraparoundDiagonal) {
    GameManager manager;

    auto surface = std::make_unique<CylindricalSurface>(0, 8);
    surface->initializeBoard();

    auto bishop = std::make_unique<Bishop>(PieceColor::WHITE, BoardPosition(0, 7, 2));
    surface->addPiece(std::move(bishop), 7, 2);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    auto piece = manager.gameBoard.getPieceAt(BoardPosition(0, 7, 2));
    auto moves = piece->getPossibleMoves(&manager.gameBoard);

    EXPECT_GT(moves.size(), 4);
}

TEST_F(NonEuclideanSurfaceTest, QueenSpherePoleMovement) {
    GameManager manager;

    auto surface = std::make_unique<SphericalSurface>(0);
    surface->initializeBoard();

    auto queen = std::make_unique<Queen>(PieceColor::WHITE, BoardPosition(0, 2, 7));
    surface->addPiece(std::move(queen), 2, 7);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(player1, player2);

    auto piece = manager.gameBoard.getPieceAt(BoardPosition(0, 2, 7));
    auto moves = piece->getPossibleMoves(&manager.gameBoard);

    bool canMoveAlongPole = false;
    for (const auto& move : moves) {
        if (move.getTo().getY() == 7 && move.getTo().getX() != 2) {
            canMoveAlongPole = true;
        }
    }

    EXPECT_TRUE(canMoveAlongPole);
}

