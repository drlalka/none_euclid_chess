#include <gtest/gtest.h>
#include "../include/container/Grid2D.hpp"
#include <vector>
#include <string>

TEST(Grid2DCylindricalTest, WrapAroundX) {
    Grid2D<int> grid;
    const int width = 8;
    const int height = 3;

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            grid.insert(x, y, x * 10 + y);
        }
    }

    for (int y = 0; y < height; ++y) {
        auto* leftEdge = grid.getNode(0, y);
        auto* rightEdge = grid.getNode(width - 1, y);

        ASSERT_NE(leftEdge, nullptr);
        ASSERT_NE(rightEdge, nullptr);

        leftEdge->setNeighbor(Grid2DNode<int>::LEFT, rightEdge);
        rightEdge->setNeighbor(Grid2DNode<int>::RIGHT, leftEdge);

        if (y > 0) {
            auto* leftEdgeDown = grid.getNode(0, y - 1);
            auto* rightEdgeDown = grid.getNode(width - 1, y - 1);

            leftEdge->setNeighbor(Grid2DNode<int>::LEFT_DOWN, rightEdgeDown);
            rightEdge->setNeighbor(Grid2DNode<int>::RIGHT_DOWN, leftEdgeDown);
        }

        if (y < height - 1) {
            auto* leftEdgeUp = grid.getNode(0, y + 1);
            auto* rightEdgeUp = grid.getNode(width - 1, y + 1);

            leftEdge->setNeighbor(Grid2DNode<int>::LEFT_UP, rightEdgeUp);
            rightEdge->setNeighbor(Grid2DNode<int>::RIGHT_UP, leftEdgeUp);
        }
    }

    auto* node = grid.getNode(7, 1);
    ASSERT_NE(node, nullptr);

    auto* rightNeighbor = node->getNeighbor(Grid2DNode<int>::RIGHT);
    ASSERT_NE(rightNeighbor, nullptr);
    EXPECT_EQ(rightNeighbor->getX(), 0);
    EXPECT_EQ(rightNeighbor->getY(), 1);

    auto* node0 = grid.getNode(0, 1);
    auto* leftNeighbor = node0->getNeighbor(Grid2DNode<int>::LEFT);
    ASSERT_NE(leftNeighbor, nullptr);
    EXPECT_EQ(leftNeighbor->getX(), 7);
    EXPECT_EQ(leftNeighbor->getY(), 1);
}

TEST(Grid2DSphericalTest, WrapAroundBoth) {
    Grid2D<std::string> grid;
    const int size = 4;

    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            grid.insert(x, y, std::to_string(x) + "," + std::to_string(y));
        }
    }

    for (int x = 0; x < size; ++x) {
        auto* topEdge = grid.getNode(x, size - 1);
        auto* bottomEdge = grid.getNode(x, 0);

        ASSERT_NE(topEdge, nullptr);
        ASSERT_NE(bottomEdge, nullptr);

        topEdge->setNeighbor(Grid2DNode<std::string>::UP, bottomEdge);
        bottomEdge->setNeighbor(Grid2DNode<std::string>::DOWN, topEdge);
    }

    for (int y = 0; y < size; ++y) {
        auto* leftEdge = grid.getNode(0, y);
        auto* rightEdge = grid.getNode(size - 1, y);

        ASSERT_NE(leftEdge, nullptr);
        ASSERT_NE(rightEdge, nullptr);

        leftEdge->setNeighbor(Grid2DNode<std::string>::LEFT, rightEdge);
        rightEdge->setNeighbor(Grid2DNode<std::string>::RIGHT, leftEdge);
    }

    auto* corner = grid.getNode(0, 0);
    ASSERT_NE(corner, nullptr);

    auto* leftNeighbor = corner->getNeighbor(Grid2DNode<std::string>::LEFT);
    ASSERT_NE(leftNeighbor, nullptr);
    EXPECT_EQ(leftNeighbor->getX(), 3);

    auto* downNeighbor = corner->getNeighbor(Grid2DNode<std::string>::DOWN);
    ASSERT_NE(downNeighbor, nullptr);
    EXPECT_EQ(downNeighbor->getY(), 3);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

