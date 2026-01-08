#include <gtest/gtest.h>
#include <chrono>
#include "../include/board/GameBoard.hpp"
#include "../include/board/surfaces/StandardSurface.hpp"
#include "../include/pieces/Pawn.hpp"
#include "../include/pieces/Rook.hpp"
#include "../include/pieces/Knight.hpp"

TEST(PortalTest, PawnSinglePortalMove) {
    GameBoard board;
    auto surface = std::make_unique<StandardSurface>(0, 16, 16);
    surface->initializeBoard();

    surface->addPiece(std::make_unique<Pawn>(PieceColor::WHITE, BoardPosition(0, 5, 5)), 5, 5);

    surface->addPortal(5, 6, 0, 10, 10);

    board.addSurface(std::move(surface));

    auto* pawn = board.getPieceAt(BoardPosition(0, 5, 5));
    ASSERT_NE(pawn, nullptr);

    auto moves = pawn->getPossibleMoves(&board);

    bool hasPortalMove = false;
    for (const auto& move : moves) {
        if (move.getTo() == BoardPosition(0, 10, 10)) {
            hasPortalMove = true;
            EXPECT_TRUE(move.isPortalUsed());
            break;
        }
    }

    EXPECT_TRUE(hasPortalMove);
}

TEST(PortalTest, RookMultiplePortals) {
    GameBoard board;
    auto surface = std::make_unique<StandardSurface>(0, 16, 16);
    surface->initializeBoard();

    surface->addPiece(std::make_unique<Rook>(PieceColor::WHITE, BoardPosition(0, 5, 5)), 5, 5);

    surface->addPortal(5, 6, 0, 10, 10);
    surface->addPortal(10, 11, 0, 3, 3);

    board.addSurface(std::move(surface));

    auto* rook = board.getPieceAt(BoardPosition(0, 5, 5));
    ASSERT_NE(rook, nullptr);

    auto moves = rook->getPossibleMoves(&board);

    bool hasSecondPortalMove = false;
    for (const auto& move : moves) {
        if (move.getTo() == BoardPosition(0, 3, 3)) {
            hasSecondPortalMove = true;
            break;
        }
    }

    EXPECT_TRUE(hasSecondPortalMove);
}

TEST(PortalTest, KnightTwoPortalsPlusMove) {
    GameBoard board;
    auto surface = std::make_unique<StandardSurface>(0, 16, 16);
    surface->initializeBoard();

    surface->addPiece(std::make_unique<Knight>(PieceColor::WHITE, BoardPosition(0, 5, 5)), 5, 5);

    surface->addPortal(5, 6, 0, 10, 10);
    surface->addPortal(10, 11, 0, 8, 8);

    board.addSurface(std::move(surface));

    auto* knight = board.getPieceAt(BoardPosition(0, 5, 5));
    ASSERT_NE(knight, nullptr);

    auto moves = knight->getPossibleMoves(&board);

    int knightMovesFromPortalExit = 0;
    for (const auto& move : moves) {
        int dx = std::abs(move.getTo().getX() - 8);
        int dy = std::abs(move.getTo().getY() - 8);

        if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2)) {
            knightMovesFromPortalExit++;
        }
    }

    EXPECT_GT(knightMovesFromPortalExit, 0);
}

TEST(PortalTest, InactivePortalNotUsed) {
    GameBoard board;
    auto surface = std::make_unique<StandardSurface>(0, 16, 16);
    surface->initializeBoard();

    surface->addPiece(std::make_unique<Pawn>(PieceColor::WHITE, BoardPosition(0, 5, 5)), 5, 5);

    surface->addPortal(5, 6, 0, 10, 10);

    auto* cell = surface->getCell(5, 6);
    ASSERT_NE(cell, nullptr);
    auto* portal = cell->getPortalLink();
    ASSERT_NE(portal, nullptr);
    portal->deactivate();

    board.addSurface(std::move(surface));

    auto* pawn = board.getPieceAt(BoardPosition(0, 5, 5));
    auto moves = pawn->getPossibleMoves(&board);

    for (const auto& move : moves) {
        EXPECT_NE(move.getTo(), BoardPosition(0, 10, 10));
    }
}

TEST(PortalTest, PortalBetweenSurfaces) {
    GameBoard board;

    auto surface1 = std::make_unique<StandardSurface>(0, 16, 16);
    surface1->initializeBoard();
    surface1->addPiece(std::make_unique<Rook>(PieceColor::WHITE, BoardPosition(0, 5, 5)), 5, 5);
    surface1->addPortal(5, 6, 1, 8, 8);

    auto surface2 = std::make_unique<StandardSurface>(1, 16, 16);
    surface2->initializeBoard();

    board.addSurface(std::move(surface1));
    board.addSurface(std::move(surface2));

    auto* rook = board.getPieceAt(BoardPosition(0, 5, 5));
    ASSERT_NE(rook, nullptr);

    auto moves = rook->getPossibleMoves(&board);

    bool hasCrossSurfaceMove = false;
    for (const auto& move : moves) {
        if (move.getTo().getSurfaceId() == 1 &&
            move.getTo().getX() == 8 &&
            move.getTo().getY() == 8) {
            hasCrossSurfaceMove = true;
            break;
        }
    }

    EXPECT_TRUE(hasCrossSurfaceMove);
}

TEST(PortalTest, PawnCannotUseTwoPortals) {
    GameBoard board;
    auto surface = std::make_unique<StandardSurface>(0, 16, 16);
    surface->initializeBoard();

    surface->addPiece(std::make_unique<Pawn>(PieceColor::WHITE, BoardPosition(0, 5, 5)), 5, 5);

    surface->addPortal(5, 6, 0, 10, 10);
    surface->addPortal(10, 11, 0, 3, 3);

    board.addSurface(std::move(surface));

    auto* pawn = board.getPieceAt(BoardPosition(0, 5, 5));
    auto moves = pawn->getPossibleMoves(&board);

    for (const auto& move : moves) {
        EXPECT_NE(move.getTo(), BoardPosition(0, 3, 3));
    }
}

TEST(PortalTest, NoInfiniteLoopWithCircularPortals) {
    GameBoard board;
    auto surface = std::make_unique<StandardSurface>(0, 16, 16);
    surface->initializeBoard();

    surface->addPiece(std::make_unique<Rook>(PieceColor::WHITE, BoardPosition(0, 5, 5)), 5, 5);

    surface->addPortal(5, 6, 0, 10, 10);
    surface->addPortal(10, 11, 0, 5, 6);

    board.addSurface(std::move(surface));

    auto* rook = board.getPieceAt(BoardPosition(0, 5, 5));

    auto start = std::chrono::steady_clock::now();
    auto moves = rook->getPossibleMoves(&board);
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    EXPECT_LT(duration.count(), 100);
    EXPECT_GT(moves.size(), 0);
}

TEST(PortalTest, MultiSurfacePortalChain) {
    GameBoard board;

    auto surface1 = std::make_unique<StandardSurface>(0, 16, 16);
    surface1->initializeBoard();
    surface1->addPiece(std::make_unique<Rook>(PieceColor::WHITE, BoardPosition(0, 5, 5)), 5, 5);
    surface1->addPortal(5, 6, 1, 8, 8);

    auto surface2 = std::make_unique<StandardSurface>(1, 16, 16);
    surface2->initializeBoard();
    surface2->addPortal(8, 9, 2, 3, 3);

    auto surface3 = std::make_unique<StandardSurface>(2, 16, 16);
    surface3->initializeBoard();

    board.addSurface(std::move(surface1));
    board.addSurface(std::move(surface2));
    board.addSurface(std::move(surface3));

    auto* rook = board.getPieceAt(BoardPosition(0, 5, 5));
    ASSERT_NE(rook, nullptr);

    auto moves = rook->getPossibleMoves(&board);

    bool hasThirdSurface = false;
    for (const auto& move : moves) {
        if (move.getTo().getSurfaceId() == 2) {
            hasThirdSurface = true;
            break;
        }
    }

    EXPECT_TRUE(hasThirdSurface);
}

TEST(PortalTest, PortalToOccupiedCell) {
    GameBoard board;
    auto surface = std::make_unique<StandardSurface>(0, 16, 16);
    surface->initializeBoard();

    surface->addPiece(std::make_unique<Rook>(PieceColor::WHITE, BoardPosition(0, 5, 5)), 5, 5);
    surface->addPiece(std::make_unique<Pawn>(PieceColor::BLACK, BoardPosition(0, 10, 10)), 10, 10);

    surface->addPortal(5, 6, 0, 10, 10);

    board.addSurface(std::move(surface));

    auto* rook = board.getPieceAt(BoardPosition(0, 5, 5));
    auto moves = rook->getPossibleMoves(&board);

    bool hasPortalMove = false;
    for (const auto& move : moves) {
        if (move.getTo() == BoardPosition(0, 10, 10) && move.isPortalUsed()) {
            hasPortalMove = true;
            break;
        }
    }

    EXPECT_TRUE(hasPortalMove);
}


