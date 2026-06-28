#include <gtest/gtest.h>
#include "../include/board/surfaces/StandardSurface.hpp"
#include "../include/pieces/King.hpp"
#include "../include/pieces/Pawn.hpp"

TEST(StandardSurfaceTest, ConstructorValidDimensions) {
    EXPECT_NO_THROW(StandardSurface(0, 8, 8));
    EXPECT_NO_THROW(StandardSurface(0, 10, 8));
    EXPECT_NO_THROW(StandardSurface(0, 16, 12));
}

TEST(StandardSurfaceTest, ConstructorInvalidDimensions) {
    EXPECT_THROW(StandardSurface(0, 7, 8), std::invalid_argument);
    EXPECT_THROW(StandardSurface(0, 8, 7), std::invalid_argument);
    EXPECT_THROW(StandardSurface(0, 9, 8), std::invalid_argument);
    EXPECT_THROW(StandardSurface(0, 8, 9), std::invalid_argument);
    EXPECT_THROW(StandardSurface(0, 6, 6), std::invalid_argument);
}

TEST(StandardSurfaceTest, GetDimensions) {
    StandardSurface surface(0, 12, 10);
    EXPECT_EQ(surface.getWidth(), 12);
    EXPECT_EQ(surface.getHeight(), 10);
}

TEST(StandardSurfaceTest, ValidCoordinates) {
    StandardSurface surface(0, 10, 8);

    EXPECT_TRUE(surface.isValidCoordinate(0, 0));
    EXPECT_TRUE(surface.isValidCoordinate(9, 7));
    EXPECT_TRUE(surface.isValidCoordinate(5, 4));

    EXPECT_FALSE(surface.isValidCoordinate(-1, 0));
    EXPECT_FALSE(surface.isValidCoordinate(10, 0));
    EXPECT_FALSE(surface.isValidCoordinate(0, -1));
    EXPECT_FALSE(surface.isValidCoordinate(0, 8));
}

TEST(StandardSurfaceTest, InitializeBoard) {
    StandardSurface surface(0, 8, 8);
    surface.initializeBoard();

    auto* cell = surface.getCell(4, 4);
    ASSERT_NE(cell, nullptr);
    EXPECT_TRUE(cell->isEmpty());
}

TEST(StandardSurfaceTest, NeighborConnections) {
    StandardSurface surface(0, 10, 10);
    surface.initializeBoard();

    auto* centerCell = surface.getCell(5, 5);
    ASSERT_NE(centerCell, nullptr);

    EXPECT_EQ(centerCell->getNeighbor(Cell::Direction::WEST), surface.getCell(4, 5));
    EXPECT_EQ(centerCell->getNeighbor(Cell::Direction::SOUTHWEST), surface.getCell(4, 4));
    EXPECT_EQ(centerCell->getNeighbor(Cell::Direction::SOUTH), surface.getCell(5, 4));
    EXPECT_EQ(centerCell->getNeighbor(Cell::Direction::SOUTHEAST), surface.getCell(6, 4));
    EXPECT_EQ(centerCell->getNeighbor(Cell::Direction::EAST), surface.getCell(6, 5));
    EXPECT_EQ(centerCell->getNeighbor(Cell::Direction::NORTHEAST), surface.getCell(6, 6));
    EXPECT_EQ(centerCell->getNeighbor(Cell::Direction::NORTH), surface.getCell(5, 6));
    EXPECT_EQ(centerCell->getNeighbor(Cell::Direction::NORTHWEST), surface.getCell(4, 6));
}

TEST(StandardSurfaceTest, EdgeCellNeighbors) {
    StandardSurface surface(0, 8, 8);
    surface.initializeBoard();

    auto* corner = surface.getCell(0, 0);
    ASSERT_NE(corner, nullptr);

    EXPECT_EQ(corner->getNeighbor(Cell::Direction::WEST), nullptr);
    EXPECT_EQ(corner->getNeighbor(Cell::Direction::SOUTHWEST), nullptr);
    EXPECT_EQ(corner->getNeighbor(Cell::Direction::SOUTH), nullptr);
    EXPECT_EQ(corner->getNeighbor(Cell::Direction::SOUTHEAST), nullptr);
    EXPECT_EQ(corner->getNeighbor(Cell::Direction::EAST), surface.getCell(1, 0));
    EXPECT_EQ(corner->getNeighbor(Cell::Direction::NORTHEAST), surface.getCell(1, 1));
    EXPECT_EQ(corner->getNeighbor(Cell::Direction::NORTH), surface.getCell(0, 1));
    EXPECT_EQ(corner->getNeighbor(Cell::Direction::NORTHWEST), nullptr);
}

TEST(StandardSurfaceTest, SetupInitialPosition) {
    StandardSurface surface(0, 8, 8);
    surface.initializeBoard();
    surface.setupInitialPosition();

    auto* whiteKing = surface.getPieceAt(4, 0);
    ASSERT_NE(whiteKing, nullptr);
    EXPECT_EQ(whiteKing->getType(), PieceType::KING);
    EXPECT_EQ(whiteKing->getColor(), PieceColor::WHITE);

    auto* blackKing = surface.getPieceAt(4, 7);
    ASSERT_NE(blackKing, nullptr);
    EXPECT_EQ(blackKing->getType(), PieceType::KING);
    EXPECT_EQ(blackKing->getColor(), PieceColor::BLACK);

    auto* whitePawn = surface.getPieceAt(0, 1);
    ASSERT_NE(whitePawn, nullptr);
    EXPECT_EQ(whitePawn->getType(), PieceType::PAWN);
    EXPECT_EQ(whitePawn->getColor(), PieceColor::WHITE);

    auto* blackPawn = surface.getPieceAt(0, 6);
    ASSERT_NE(blackPawn, nullptr);
    EXPECT_EQ(blackPawn->getType(), PieceType::PAWN);
    EXPECT_EQ(blackPawn->getColor(), PieceColor::BLACK);
}

TEST(StandardSurfaceTest, LargeBoardSetup) {
    StandardSurface surface(0, 16, 12);
    surface.initializeBoard();
    surface.setupInitialPosition();

    int whitePieceCount = 0;
    int blackPieceCount = 0;

    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 12; y++) {
            auto* piece = surface.getPieceAt(x, y);
            if (piece) {
                if (piece->getColor() == PieceColor::WHITE) whitePieceCount++;
                else blackPieceCount++;
            }
        }
    }

    EXPECT_EQ(whitePieceCount, 32);
    EXPECT_EQ(blackPieceCount, 32);
}

TEST(StandardSurfaceTest, AddAndRemovePiece) {
    StandardSurface surface(0, 8, 8);
    surface.initializeBoard();

    auto piece = std::make_unique<King>(PieceColor::WHITE, BoardPosition(0, 3, 3));
    surface.addPiece(std::move(piece), 3, 3);

    auto* retrievedPiece = surface.getPieceAt(3, 3);
    ASSERT_NE(retrievedPiece, nullptr);
    EXPECT_EQ(retrievedPiece->getType(), PieceType::KING);

    auto removedPiece = surface.removePiece(3, 3);
    ASSERT_NE(removedPiece, nullptr);
    EXPECT_EQ(removedPiece->getType(), PieceType::KING);

    EXPECT_EQ(surface.getPieceAt(3, 3), nullptr);
}

