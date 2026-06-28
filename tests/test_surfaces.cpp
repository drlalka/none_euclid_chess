#include <gtest/gtest.h>
#include "../include/board/surfaces/RectangularSurface.hpp"
#include "../include/board/surfaces/CylindricalSurface.hpp"
#include "../include/board/surfaces/SphericalSurface.hpp"
#include "../include/pieces/Rook.hpp"
#include "../include/pieces/Pawn.hpp"
#include "../include/pieces/King.hpp"

class SurfaceTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(SurfaceTest, RectangularSurfaceCreation) {
    RectangularSurface surface(0, 5);

    EXPECT_EQ(surface.getSurfaceId(), 0);
    EXPECT_EQ(surface.getWidth(), 8);
    EXPECT_EQ(surface.getHeight(), 5);
}

TEST_F(SurfaceTest, RectangularSurfaceInitialization) {
    RectangularSurface surface(1, 6);
    surface.initializeBoard();

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 6; y++) {
            Cell* cell = surface.getCell(x, y);
            EXPECT_NE(cell, nullptr);
            EXPECT_EQ(cell->getPosition().getX(), x);
            EXPECT_EQ(cell->getPosition().getY(), y);
            EXPECT_EQ(cell->getPosition().getSurfaceId(), 1);
        }
    }
}

TEST_F(SurfaceTest, RectangularSurfaceValidCoordinates) {
    RectangularSurface surface(0, 8);
    surface.initializeBoard();

    EXPECT_TRUE(surface.isValidCoordinate(0, 0));
    EXPECT_TRUE(surface.isValidCoordinate(7, 7));
    EXPECT_TRUE(surface.isValidCoordinate(4, 4));

    EXPECT_FALSE(surface.isValidCoordinate(-1, 0));
    EXPECT_FALSE(surface.isValidCoordinate(0, -1));
    EXPECT_FALSE(surface.isValidCoordinate(8, 0));
    EXPECT_FALSE(surface.isValidCoordinate(0, 8));
    EXPECT_FALSE(surface.isValidCoordinate(10, 10));
}

TEST_F(SurfaceTest, RectangularSurfaceAddPiece) {
    RectangularSurface surface(0, 8);
    surface.initializeBoard();

    auto rook = std::make_unique<Rook>(PieceColor::WHITE, BoardPosition(0, 3, 3));
    surface.addPiece(std::move(rook), 3, 3);

    ChessPiece* piece = surface.getPieceAt(3, 3);
    EXPECT_NE(piece, nullptr);
    EXPECT_EQ(piece->getType(), PieceType::ROOK);
    EXPECT_EQ(piece->getColor(), PieceColor::WHITE);
}

TEST_F(SurfaceTest, RectangularSurfaceRemovePiece) {
    RectangularSurface surface(0, 8);
    surface.initializeBoard();

    auto rook = std::make_unique<Rook>(PieceColor::WHITE, BoardPosition(0, 2, 2));
    surface.addPiece(std::move(rook), 2, 2);

    EXPECT_NE(surface.getPieceAt(2, 2), nullptr);

    auto removed = surface.removePiece(2, 2);
    EXPECT_NE(removed, nullptr);
    EXPECT_EQ(removed->getType(), PieceType::ROOK);
    EXPECT_EQ(surface.getPieceAt(2, 2), nullptr);
}

TEST_F(SurfaceTest, CylindricalSurfaceCreation) {
    CylindricalSurface surface(0, 6);

    EXPECT_EQ(surface.getSurfaceId(), 0);
    EXPECT_EQ(surface.getWidth(), 8);
    EXPECT_EQ(surface.getHeight(), 6);
}

TEST_F(SurfaceTest, CylindricalSurfaceInitialization) {
    CylindricalSurface surface(2, 4);
    surface.initializeBoard();

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 4; y++) {
            Cell* cell = surface.getCell(x, y);
            EXPECT_NE(cell, nullptr);
        }
    }
}

TEST_F(SurfaceTest, CylindricalSurfaceWraparound) {
    CylindricalSurface surface(0, 8);
    surface.initializeBoard();

    Cell* cell0 = surface.getCell(0, 4);
    Cell* cell7 = surface.getCell(7, 4);

    EXPECT_NE(cell0, nullptr);
    EXPECT_NE(cell7, nullptr);

    Cell* neighbor = cell7->getNeighbor(Cell::Direction::EAST);
    EXPECT_EQ(neighbor, cell0);
}

TEST_F(SurfaceTest, CylindricalSurfaceAddAndMovePiece) {
    CylindricalSurface surface(0, 8);
    surface.initializeBoard();

    auto pawn = std::make_unique<Pawn>(PieceColor::WHITE, BoardPosition(0, 4, 1));
    surface.addPiece(std::move(pawn), 4, 1);

    ChessPiece* piece = surface.getPieceAt(4, 1);
    EXPECT_NE(piece, nullptr);
    EXPECT_EQ(piece->getType(), PieceType::PAWN);

    auto removed = surface.removePiece(4, 1);
    EXPECT_NE(removed, nullptr);
}

TEST_F(SurfaceTest, SphericalSurfaceCreation) {
    SphericalSurface surface(0);

    EXPECT_EQ(surface.getSurfaceId(), 0);
    EXPECT_EQ(surface.getWidth(), 8);
    EXPECT_EQ(surface.getHeight(), 8);
}

TEST_F(SurfaceTest, SphericalSurfaceInitialization) {
    SphericalSurface surface(3);
    surface.initializeBoard();

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            Cell* cell = surface.getCell(x, y);
            EXPECT_NE(cell, nullptr);
        }
    }
}

TEST_F(SurfaceTest, SphericalSurfaceNorthPole) {
    SphericalSurface surface(0);
    surface.initializeBoard();

    for (int x = 0; x < 8; x++) {
        Cell* cell = surface.getCell(x, 7);
        EXPECT_NE(cell, nullptr);
        EXPECT_EQ(cell->getPosition().getY(), 7);
    }
}

TEST_F(SurfaceTest, SphericalSurfaceSouthPole) {
    SphericalSurface surface(0);
    surface.initializeBoard();

    for (int x = 0; x < 8; x++) {
        Cell* cell = surface.getCell(x, 0);
        EXPECT_NE(cell, nullptr);
        EXPECT_EQ(cell->getPosition().getY(), 0);
    }
}

TEST_F(SurfaceTest, SphericalSurfaceAddPiece) {
    SphericalSurface surface(0);
    surface.initializeBoard();

    auto king = std::make_unique<King>(PieceColor::BLACK, BoardPosition(0, 4, 4));
    surface.addPiece(std::move(king), 4, 4);

    ChessPiece* piece = surface.getPieceAt(4, 4);
    EXPECT_NE(piece, nullptr);
    EXPECT_EQ(piece->getType(), PieceType::KING);
    EXPECT_EQ(piece->getColor(), PieceColor::BLACK);
}

TEST_F(SurfaceTest, SurfaceSetupInitialPosition) {
    RectangularSurface surface(0, 8);
    surface.initializeBoard();
    surface.setupInitialPosition();

    EXPECT_NE(surface.getPieceAt(0, 0), nullptr);
    EXPECT_EQ(surface.getPieceAt(0, 0)->getType(), PieceType::ROOK);
    EXPECT_EQ(surface.getPieceAt(0, 0)->getColor(), PieceColor::WHITE);

    EXPECT_NE(surface.getPieceAt(4, 0), nullptr);
    EXPECT_EQ(surface.getPieceAt(4, 0)->getType(), PieceType::KING);

    EXPECT_NE(surface.getPieceAt(0, 1), nullptr);
    EXPECT_EQ(surface.getPieceAt(0, 1)->getType(), PieceType::PAWN);

    EXPECT_NE(surface.getPieceAt(0, 7), nullptr);
    EXPECT_EQ(surface.getPieceAt(0, 7)->getType(), PieceType::ROOK);
    EXPECT_EQ(surface.getPieceAt(0, 7)->getColor(), PieceColor::BLACK);

    EXPECT_NE(surface.getPieceAt(4, 7), nullptr);
    EXPECT_EQ(surface.getPieceAt(4, 7)->getType(), PieceType::KING);
}

TEST_F(SurfaceTest, CylindricalSetupInitialPosition) {
    CylindricalSurface surface(0, 8);
    surface.initializeBoard();
    surface.setupInitialPosition();

    EXPECT_NE(surface.getPieceAt(0, 0), nullptr);
    EXPECT_NE(surface.getPieceAt(4, 0), nullptr);
    EXPECT_NE(surface.getPieceAt(0, 7), nullptr);
}

TEST_F(SurfaceTest, SphericalSetupInitialPosition) {
    SphericalSurface surface(0);
    surface.initializeBoard();
    surface.setupInitialPosition();

    EXPECT_NE(surface.getPieceAt(0, 0), nullptr);
    EXPECT_NE(surface.getPieceAt(0, 7), nullptr);
}


TEST_F(SurfaceTest, MultipleSurfacesWithDifferentIds) {
    RectangularSurface surface1(1, 8);
    CylindricalSurface surface2(2, 8);
    SphericalSurface surface3(3);

    surface1.initializeBoard();
    surface2.initializeBoard();
    surface3.initializeBoard();

    EXPECT_EQ(surface1.getSurfaceId(), 1);
    EXPECT_EQ(surface2.getSurfaceId(), 2);
    EXPECT_EQ(surface3.getSurfaceId(), 3);
}

TEST_F(SurfaceTest, SurfaceEmptyCellCheck) {
    RectangularSurface surface(0, 8);
    surface.initializeBoard();

    EXPECT_EQ(surface.getPieceAt(4, 4), nullptr);

    auto rook = std::make_unique<Rook>(PieceColor::WHITE, BoardPosition(0, 4, 4));
    surface.addPiece(std::move(rook), 4, 4);

    EXPECT_NE(surface.getPieceAt(4, 4), nullptr);
}

