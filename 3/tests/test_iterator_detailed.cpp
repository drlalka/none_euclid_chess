#include <gtest/gtest.h>
#include "../include/container/Grid2D.hpp"
#include <vector>
#include <algorithm>

TEST(Grid2DIteratorTest, BasicIteration) {
    Grid2D<int> grid;

    grid.insert(0, 0, 1);
    grid.insert(1, 0, 2);
    grid.insert(0, 1, 3);
    grid.insert(1, 1, 4);

    std::vector<int> values;
    for (const auto& val : grid) {
        values.push_back(val);
    }

    EXPECT_EQ(values.size(), 4);
    EXPECT_TRUE(std::find(values.begin(), values.end(), 1) != values.end());
    EXPECT_TRUE(std::find(values.begin(), values.end(), 2) != values.end());
    EXPECT_TRUE(std::find(values.begin(), values.end(), 3) != values.end());
    EXPECT_TRUE(std::find(values.begin(), values.end(), 4) != values.end());
}

TEST(Grid2DIteratorTest, IterationOrderBySortedCoordinates) {
    Grid2D<std::string> grid;

    grid.insert(1, 1, "B");
    grid.insert(0, 0, "A");
    grid.insert(2, 0, "C");
    grid.insert(0, 1, "D");

    std::vector<std::string> values;
    for (const auto& val : grid) {
        values.push_back(val);
    }

    ASSERT_EQ(values.size(), 4);
    EXPECT_EQ(values[0], "A");
    EXPECT_EQ(values[1], "D");
    EXPECT_EQ(values[2], "B");
    EXPECT_EQ(values[3], "C");
}

TEST(Grid2DIteratorTest, ModificationThroughIterator) {
    Grid2D<int> grid;

    grid.insert(0, 0, 10);
    grid.insert(1, 0, 20);
    grid.insert(2, 0, 30);

    for (auto& value : grid) {
        value *= 2;
    }

    EXPECT_EQ(grid.at(0, 0), 20);
    EXPECT_EQ(grid.at(1, 0), 40);
    EXPECT_EQ(grid.at(2, 0), 60);
}

TEST(Grid2DIteratorTest, ExplicitBeginEnd) {
    Grid2D<int> grid;

    grid.insert(0, 0, 1);
    grid.insert(1, 0, 2);
    grid.insert(2, 0, 3);

    int sum = 0;
    for (auto it = grid.begin(); it != grid.end(); ++it) {
        sum += *it;
    }

    EXPECT_EQ(sum, 6);
}

TEST(Grid2DIteratorTest, ConstIterator) {
    Grid2D<int> grid;

    grid.insert(0, 0, 5);
    grid.insert(1, 0, 10);

    const Grid2D<int>& const_grid = grid;

    int count = 0;
    for (auto it = const_grid.begin(); it != const_grid.end(); ++it) {
        count++;
    }

    EXPECT_EQ(count, 2);
}

TEST(Grid2DIteratorTest, CBeginCEnd) {
    Grid2D<int> grid;

    grid.insert(0, 0, 1);
    grid.insert(1, 0, 2);

    int product = 1;
    for (auto it = grid.cbegin(); it != grid.cend(); ++it) {
        product *= *it;
    }

    EXPECT_EQ(product, 2);
}

TEST(Grid2DIteratorTest, STLAlgorithmFind) {
    Grid2D<int> grid;

    grid.insert(0, 0, 10);
    grid.insert(1, 0, 20);
    grid.insert(2, 0, 30);
    grid.insert(3, 0, 40);

    auto it = std::find(grid.begin(), grid.end(), 30);

    ASSERT_NE(it, grid.end());
    EXPECT_EQ(*it, 30);
}

TEST(Grid2DIteratorTest, STLAlgorithmMaxElement) {
    Grid2D<int> grid;

    grid.insert(0, 0, 15);
    grid.insert(1, 0, 42);
    grid.insert(2, 0, 8);
    grid.insert(3, 0, 23);

    auto max_it = std::max_element(grid.begin(), grid.end());

    ASSERT_NE(max_it, grid.end());
    EXPECT_EQ(*max_it, 42);
}

TEST(Grid2DIteratorTest, STLAlgorithmCount) {
    Grid2D<int> grid;

    grid.insert(0, 0, 5);
    grid.insert(1, 0, 10);
    grid.insert(2, 0, 5);
    grid.insert(3, 0, 15);
    grid.insert(4, 0, 5);

    int count = std::count(grid.begin(), grid.end(), 5);

    EXPECT_EQ(count, 3);
}

TEST(Grid2DIteratorTest, STLAlgorithmTransform) {
    Grid2D<int> grid;

    grid.insert(0, 0, 1);
    grid.insert(1, 0, 2);
    grid.insert(2, 0, 3);

    std::vector<int> doubled;
    std::transform(grid.begin(), grid.end(), std::back_inserter(doubled),
                   [](int val) { return val * 2; });

    ASSERT_EQ(doubled.size(), 3);
    EXPECT_TRUE(std::find(doubled.begin(), doubled.end(), 2) != doubled.end());
    EXPECT_TRUE(std::find(doubled.begin(), doubled.end(), 4) != doubled.end());
    EXPECT_TRUE(std::find(doubled.begin(), doubled.end(), 6) != doubled.end());
}

TEST(Grid2DIteratorTest, IterationVsGraphTraversal) {
    Grid2D<char> grid;

    grid.insert(0, 0, 'A');
    grid.insert(2, 0, 'B');
    grid.insert(1, 1, 'C');
    grid.insert(3, 2, 'D');

    auto* nodeA = grid.getNode(0, 0);
    auto* nodeB = grid.getNode(2, 0);

    nodeA->setNeighbor(Grid2DNode<char>::RIGHT, nodeB);
    nodeB->setNeighbor(Grid2DNode<char>::LEFT, nodeA);

    std::vector<char> iteration_order;
    for (const auto& val : grid) {
        iteration_order.push_back(val);
    }

    ASSERT_EQ(iteration_order.size(), 4);
    EXPECT_EQ(iteration_order[0], 'A');
    EXPECT_EQ(iteration_order[1], 'C');
    EXPECT_EQ(iteration_order[2], 'B');
    EXPECT_EQ(iteration_order[3], 'D');

    std::vector<char> graph_traversal;
    graph_traversal.push_back(nodeA->getData());
    auto* neighbor = nodeA->getNeighbor(Grid2DNode<char>::RIGHT);
    if (neighbor) {
        graph_traversal.push_back(neighbor->getData());
    }

    ASSERT_EQ(graph_traversal.size(), 2);
    EXPECT_EQ(graph_traversal[0], 'A');
    EXPECT_EQ(graph_traversal[1], 'B');
}

TEST(Grid2DIteratorTest, EmptyGridIteration) {
    Grid2D<int> grid;

    int count = 0;
    for (const auto& val : grid) {
        (void)val;
        count++;
    }

    EXPECT_EQ(count, 0);
    EXPECT_EQ(grid.begin(), grid.end());
}

TEST(Grid2DIteratorTest, SingleElementIteration) {
    Grid2D<std::string> grid;

    grid.insert(5, 5, "single");

    int count = 0;
    std::string value;
    for (const auto& val : grid) {
        value = val;
        count++;
    }

    EXPECT_EQ(count, 1);
    EXPECT_EQ(value, "single");
}

TEST(Grid2DIteratorTest, IterationAfterErase) {
    Grid2D<int> grid;

    grid.insert(0, 0, 1);
    grid.insert(1, 0, 2);
    grid.insert(2, 0, 3);

    grid.erase(1, 0);

    std::vector<int> values;
    for (const auto& val : grid) {
        values.push_back(val);
    }

    ASSERT_EQ(values.size(), 2);
    EXPECT_EQ(values[0], 1);
    EXPECT_EQ(values[1], 3);
}

TEST(Grid2DIteratorTest, LargeGridIteration) {
    Grid2D<int> grid;

    const int size = 10;
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            grid.insert(x, y, x * size + y);
        }
    }

    int count = 0;
    for (const auto& val : grid) {
        (void)val;
        count++;
    }

    EXPECT_EQ(count, size * size);
}

TEST(Grid2DIteratorTest, IteratorGetXY) {
    Grid2D<int> grid;

    grid.insert(3, 7, 42);

    auto it = grid.begin();

    EXPECT_EQ(it.getX(), 3);
    EXPECT_EQ(it.getY(), 7);
    EXPECT_EQ(*it, 42);
}

TEST(Grid2DIteratorTest, IteratorGetNode) {
    Grid2D<int> grid;

    grid.insert(1, 2, 100);

    auto it = grid.begin();
    auto* node = it.getNode();

    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->getData(), 100);
    EXPECT_EQ(node->getX(), 1);
    EXPECT_EQ(node->getY(), 2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

