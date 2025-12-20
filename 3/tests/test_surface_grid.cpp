#include <gtest/gtest.h>
#include "../include/container/Grid2D.hpp"
#include "../include/container/Grid2DNode.hpp"
#include "../include/container/Grid2DIterator.hpp"
#include <string>
#include <vector>

TEST(Grid2DTest, DefaultConstructor) {
    Grid2D<int> grid;
    EXPECT_TRUE(grid.empty());
    EXPECT_EQ(grid.size(), 0);
}

TEST(Grid2DTest, InsertElements) {
    Grid2D<int> grid;

    grid.insert(0, 0, 10);
    grid.insert(1, 0, 20);
    grid.insert(0, 1, 30);

    EXPECT_FALSE(grid.empty());
    EXPECT_EQ(grid.size(), 3);
    EXPECT_TRUE(grid.contains(0, 0));
    EXPECT_TRUE(grid.contains(1, 0));
    EXPECT_TRUE(grid.contains(0, 1));
    EXPECT_FALSE(grid.contains(2, 2));
}

TEST(Grid2DTest, AccessElements) {
    Grid2D<int> grid;

    grid.insert(5, 5, 42);

    EXPECT_EQ(grid.at(5, 5), 42);
    EXPECT_EQ(grid(5, 5), 42);

    grid(5, 5) = 100;
    EXPECT_EQ(grid.at(5, 5), 100);
}

TEST(Grid2DTest, AtThrowsException) {
    Grid2D<int> grid;

    EXPECT_THROW(grid.at(0, 0), std::out_of_range);

    grid.insert(0, 0, 10);
    EXPECT_NO_THROW(grid.at(0, 0));
    EXPECT_THROW(grid.at(1, 1), std::out_of_range);
}

TEST(Grid2DTest, NeighborLinks) {
    Grid2D<int> grid;

    for (int x = 0; x < 3; ++x) {
        for (int y = 0; y < 3; ++y) {
            grid.insert(x, y, x * 10 + y);
        }
    }

    auto* centerNode = grid.getNode(1, 1);
    ASSERT_NE(centerNode, nullptr);

    EXPECT_NE(centerNode->getNeighbor(Grid2DNode<int>::RIGHT), nullptr);
    EXPECT_NE(centerNode->getNeighbor(Grid2DNode<int>::LEFT), nullptr);
    EXPECT_NE(centerNode->getNeighbor(Grid2DNode<int>::UP), nullptr);
    EXPECT_NE(centerNode->getNeighbor(Grid2DNode<int>::DOWN), nullptr);
    EXPECT_NE(centerNode->getNeighbor(Grid2DNode<int>::RIGHT_UP), nullptr);
    EXPECT_NE(centerNode->getNeighbor(Grid2DNode<int>::LEFT_UP), nullptr);
    EXPECT_NE(centerNode->getNeighbor(Grid2DNode<int>::RIGHT_DOWN), nullptr);
    EXPECT_NE(centerNode->getNeighbor(Grid2DNode<int>::LEFT_DOWN), nullptr);

    EXPECT_EQ(centerNode->getNeighbor(Grid2DNode<int>::RIGHT)->getData(), 21);  // (2,1)
    EXPECT_EQ(centerNode->getNeighbor(Grid2DNode<int>::LEFT)->getData(), 1);    // (0,1)
    EXPECT_EQ(centerNode->getNeighbor(Grid2DNode<int>::UP)->getData(), 12);     // (1,2)
    EXPECT_EQ(centerNode->getNeighbor(Grid2DNode<int>::DOWN)->getData(), 10);   // (1,0)
}

// Тест углового узла (меньше соседей)
TEST(Grid2DTest, CornerNodeNeighbors) {
    Grid2D<int> grid;

    grid.insert(0, 0, 1);
    grid.insert(1, 0, 2);
    grid.insert(0, 1, 3);
    grid.insert(1, 1, 4);

    auto* cornerNode = grid.getNode(0, 0);
    ASSERT_NE(cornerNode, nullptr);

    // У углового узла должно быть только 3 соседа
    EXPECT_NE(cornerNode->getNeighbor(Grid2DNode<int>::RIGHT), nullptr);
    EXPECT_NE(cornerNode->getNeighbor(Grid2DNode<int>::UP), nullptr);
    EXPECT_NE(cornerNode->getNeighbor(Grid2DNode<int>::RIGHT_UP), nullptr);

    // Остальные направления должны быть пусты
    EXPECT_EQ(cornerNode->getNeighbor(Grid2DNode<int>::LEFT), nullptr);
    EXPECT_EQ(cornerNode->getNeighbor(Grid2DNode<int>::DOWN), nullptr);
}

// Тест удаления элементов
TEST(Grid2DTest, EraseElements) {
    Grid2D<int> grid;

    grid.insert(0, 0, 10);
    grid.insert(1, 0, 20);
    grid.insert(2, 0, 30);

    EXPECT_EQ(grid.size(), 3);
    EXPECT_TRUE(grid.contains(1, 0));

    grid.erase(1, 0);

    EXPECT_EQ(grid.size(), 2);
    EXPECT_FALSE(grid.contains(1, 0));

    // Проверяем, что связи обновились
    auto* node0 = grid.getNode(0, 0);
    auto* node2 = grid.getNode(2, 0);

    EXPECT_EQ(node0->getNeighbor(Grid2DNode<int>::RIGHT), nullptr);
}

// Тест очистки
TEST(Grid2DTest, ClearGrid) {
    Grid2D<int> grid;

    for (int i = 0; i < 10; ++i) {
        grid.insert(i, 0, i * 10);
    }

    EXPECT_EQ(grid.size(), 10);

    grid.clear();

    EXPECT_TRUE(grid.empty());
    EXPECT_EQ(grid.size(), 0);
}

// Тест swap
TEST(Grid2DTest, SwapGrids) {
    Grid2D<int> grid1;
    Grid2D<int> grid2;

    grid1.insert(0, 0, 10);
    grid2.insert(1, 1, 20);
    grid2.insert(2, 2, 30);

    EXPECT_EQ(grid1.size(), 1);
    EXPECT_EQ(grid2.size(), 2);

    grid1.swap(grid2);

    EXPECT_EQ(grid1.size(), 2);
    EXPECT_EQ(grid2.size(), 1);
    EXPECT_TRUE(grid1.contains(1, 1));
    EXPECT_TRUE(grid2.contains(0, 0));
}

// Тест копирования
TEST(Grid2DTest, CopyConstructor) {
    Grid2D<int> grid1;

    grid1.insert(0, 0, 10);
    grid1.insert(1, 0, 20);
    grid1.insert(0, 1, 30);

    Grid2D<int> grid2(grid1);

    EXPECT_EQ(grid2.size(), 3);
    EXPECT_EQ(grid2.at(0, 0), 10);
    EXPECT_EQ(grid2.at(1, 0), 20);
    EXPECT_EQ(grid2.at(0, 1), 30);

    // Изменение grid2 не должно влиять на grid1
    grid2(0, 0) = 999;
    EXPECT_EQ(grid1.at(0, 0), 10);
    EXPECT_EQ(grid2.at(0, 0), 999);
}

// Тест перемещения
TEST(Grid2DTest, MoveConstructor) {
    Grid2D<int> grid1;

    grid1.insert(0, 0, 10);
    grid1.insert(1, 0, 20);

    Grid2D<int> grid2(std::move(grid1));

    EXPECT_EQ(grid2.size(), 2);
    EXPECT_EQ(grid1.size(), 0); // grid1 должен быть пуст после перемещения
    EXPECT_TRUE(grid1.empty());
}

// Тест итераторов
TEST(Grid2DTest, Iterators) {
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

// Тест const итераторов
TEST(Grid2DTest, ConstIterators) {
    Grid2D<int> grid;

    grid.insert(0, 0, 5);
    grid.insert(1, 0, 10);
    grid.insert(2, 0, 15);

    const Grid2D<int>& constGrid = grid;

    int count = 0;
    for (auto it = constGrid.begin(); it != constGrid.end(); ++it) {
        ++count;
    }

    EXPECT_EQ(count, 3);
}

// Тест range-based for
TEST(Grid2DTest, RangeBasedFor) {
    Grid2D<int> grid;

    grid.insert(0, 0, 10);
    grid.insert(1, 0, 20);
    grid.insert(2, 0, 30);

    int sum = 0;
    for (const auto& value : grid) {
        sum += value;
    }

    EXPECT_EQ(sum, 60);
}

// Тест с пользовательским типом
TEST(Grid2DTest, CustomType) {
    struct Point {
        int x, y;
        Point(int x = 0, int y = 0) : x(x), y(y) {}
        bool operator==(const Point& other) const {
            return x == other.x && y == other.y;
        }
    };

    Grid2D<Point> grid;

    grid.insert(0, 0, Point(10, 20));
    grid.insert(1, 0, Point(30, 40));

    EXPECT_EQ(grid.at(0, 0).x, 10);
    EXPECT_EQ(grid.at(0, 0).y, 20);
    EXPECT_EQ(grid.at(1, 0).x, 30);
    EXPECT_EQ(grid.at(1, 0).y, 40);
}

// Тест с std::string
TEST(Grid2DTest, StringType) {
    Grid2D<std::string> grid;

    grid.insert(0, 0, "Hello");
    grid.insert(1, 0, "World");
    grid.insert(0, 1, "Grid2D");

    EXPECT_EQ(grid.at(0, 0), "Hello");
    EXPECT_EQ(grid.at(1, 0), "World");
    EXPECT_EQ(grid.at(0, 1), "Grid2D");
}

// Тест большой сетки
TEST(Grid2DTest, LargeGrid) {
    Grid2D<int> grid;

    const int size = 100;
    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            grid.insert(x, y, x * size + y);
        }
    }

    EXPECT_EQ(grid.size(), size * size);

    // Проверяем случайные элементы
    EXPECT_EQ(grid.at(50, 50), 50 * size + 50);
    EXPECT_EQ(grid.at(99, 99), 99 * size + 99);
}

// Тест getNeighborNodes
TEST(Grid2DTest, GetNeighborNodes) {
    Grid2D<int> grid;

    // Создаем сетку 3x3
    for (int x = 0; x < 3; ++x) {
        for (int y = 0; y < 3; ++y) {
            grid.insert(x, y, x * 10 + y);
        }
    }

    auto neighbors = grid.getNeighborNodes(1, 1);
    EXPECT_EQ(neighbors.size(), 8); // У центрального узла 8 соседей

    neighbors = grid.getNeighborNodes(0, 0);
    EXPECT_EQ(neighbors.size(), 3); // У углового узла 3 соседа
}

// Тест обновления существующего элемента
TEST(Grid2DTest, UpdateExistingElement) {
    Grid2D<int> grid;

    grid.insert(5, 5, 100);
    EXPECT_EQ(grid.at(5, 5), 100);
    EXPECT_EQ(grid.size(), 1);

    grid.insert(5, 5, 200);
    EXPECT_EQ(grid.at(5, 5), 200);
    EXPECT_EQ(grid.size(), 1);
}

TEST(Grid2DTest, OperatorParenthesesConst) {
    Grid2D<int> grid;
    grid.insert(3, 3, 99);

    const Grid2D<int>& constGrid = grid;
    EXPECT_EQ(constGrid(3, 3), 99);
}

TEST(Grid2DTest, OperatorParenthesesThrows) {
    Grid2D<int> grid;
    EXPECT_THROW(grid(5, 5), std::out_of_range);
}

TEST(Grid2DTest, CopyAssignment) {
    Grid2D<int> grid1;
    grid1.insert(0, 0, 10);
    grid1.insert(1, 1, 20);

    Grid2D<int> grid2;
    grid2.insert(5, 5, 50);

    grid2 = grid1;

    EXPECT_EQ(grid2.size(), 2);
    EXPECT_EQ(grid2.at(0, 0), 10);
    EXPECT_EQ(grid2.at(1, 1), 20);
    EXPECT_FALSE(grid2.contains(5, 5));
}

TEST(Grid2DTest, MoveAssignment) {
    Grid2D<int> grid1;
    grid1.insert(0, 0, 10);
    grid1.insert(1, 1, 20);

    Grid2D<int> grid2;
    grid2 = std::move(grid1);

    EXPECT_EQ(grid2.size(), 2);
    EXPECT_TRUE(grid1.empty());
}

TEST(Grid2DTest, SelfAssignment) {
    Grid2D<int> grid;
    grid.insert(0, 0, 10);

    grid = grid;

    EXPECT_EQ(grid.size(), 1);
    EXPECT_EQ(grid.at(0, 0), 10);
}

TEST(Grid2DTest, InsertWithMove) {
    Grid2D<std::string> grid;
    std::string str = "test";

    grid.insert(0, 0, std::move(str));
    EXPECT_EQ(grid.at(0, 0), "test");
}

TEST(Grid2DTest, UpdateWithMove) {
    Grid2D<std::string> grid;
    grid.insert(0, 0, "old");

    std::string newStr = "new";
    grid.insert(0, 0, std::move(newStr));

    EXPECT_EQ(grid.at(0, 0), "new");
    EXPECT_EQ(grid.size(), 1);
}

TEST(Grid2DTest, EraseNonExistent) {
    Grid2D<int> grid;
    grid.insert(0, 0, 10);

    grid.erase(5, 5);

    EXPECT_EQ(grid.size(), 1);
}

TEST(Grid2DTest, GetNodeNull) {
    Grid2D<int> grid;

    auto* node = grid.getNode(10, 10);
    EXPECT_EQ(node, nullptr);
}

TEST(Grid2DTest, GetNeighborNodesEmpty) {
    Grid2D<int> grid;

    auto neighbors = grid.getNeighborNodes(5, 5);
    EXPECT_TRUE(neighbors.empty());
}

TEST(Grid2DTest, ManualLinkingSimple) {
    Grid2D<int> grid;

    for (int i = 0; i < 5; ++i) {
        grid.insert(i, 0, i * 10);
    }

    auto* node4 = grid.getNode(4, 0);
    auto* node0 = grid.getNode(0, 0);

    ASSERT_NE(node4, nullptr);
    ASSERT_NE(node0, nullptr);

    node4->setNeighbor(Grid2DNode<int>::Direction::RIGHT, node0);
    node0->setNeighbor(Grid2DNode<int>::Direction::LEFT, node4);

    auto* right = node4->getNeighbor(Grid2DNode<int>::Direction::RIGHT);
    ASSERT_NE(right, nullptr);
    EXPECT_EQ(right->getX(), 0);
}

TEST(Grid2DTest, BeginEndEmpty) {
    Grid2D<int> grid;

    EXPECT_EQ(grid.begin(), grid.end());
}

TEST(Grid2DTest, CBeginCEnd) {
    Grid2D<int> grid;
    grid.insert(0, 0, 10);
    grid.insert(1, 0, 20);

    int sum = 0;
    for (auto it = grid.cbegin(); it != grid.cend(); ++it) {
        sum += *it;
    }

    EXPECT_EQ(sum, 30);
}

TEST(Grid2DTest, IteratorDecrement) {
    Grid2D<int> grid;
    grid.insert(0, 0, 10);
    grid.insert(1, 0, 20);
    grid.insert(2, 0, 30);

    auto it = grid.end();
    --it;
    EXPECT_EQ(*it, 30);

    it--;
    EXPECT_EQ(*it, 20);
}

TEST(Grid2DTest, IteratorComparison) {
    Grid2D<int> grid;
    grid.insert(0, 0, 10);

    auto it1 = grid.begin();
    auto it2 = grid.begin();

    EXPECT_TRUE(it1 == it2);
    EXPECT_FALSE(it1 != it2);

    ++it2;
    EXPECT_FALSE(it1 == it2);
    EXPECT_TRUE(it1 != it2);
}

TEST(Grid2DTest, IteratorArrow) {
    struct Data {
        int value;
        Data(int v) : value(v) {}
    };

    Grid2D<Data> grid;
    grid.insert(0, 0, Data(42));

    auto it = grid.begin();
    EXPECT_EQ(it->value, 42);
}

TEST(Grid2DTest, IteratorPostIncrement) {
    Grid2D<int> grid;
    grid.insert(0, 0, 10);
    grid.insert(1, 0, 20);

    auto it = grid.begin();
    auto old_it = it++;

    EXPECT_EQ(*old_it, 10);
    EXPECT_EQ(*it, 20);
}

TEST(Grid2DTest, ConstIteratorFromIterator) {
    Grid2D<int> grid;
    grid.insert(0, 0, 10);

    const Grid2D<int>& constGrid = grid;
    auto it = constGrid.begin();

    EXPECT_EQ(*it, 10);
}

TEST(Grid2DTest, NodeSetData) {
    Grid2D<int> grid;
    grid.insert(0, 0, 10);

    auto* node = grid.getNode(0, 0);
    node->setData(99);

    EXPECT_EQ(grid.at(0, 0), 99);
}

TEST(Grid2DTest, NodeSetDataMove) {
    Grid2D<std::string> grid;
    grid.insert(0, 0, "old");

    auto* node = grid.getNode(0, 0);
    std::string newStr = "new";
    node->setData(std::move(newStr));

    EXPECT_EQ(grid.at(0, 0), "new");
}

TEST(Grid2DTest, NodeHasNeighbor) {
    Grid2D<int> grid;
    grid.insert(0, 0, 10);
    grid.insert(1, 0, 20);

    auto* node = grid.getNode(0, 0);

    EXPECT_TRUE(node->hasNeighbor(Grid2DNode<int>::Direction::RIGHT));
    EXPECT_FALSE(node->hasNeighbor(Grid2DNode<int>::Direction::LEFT));
}

TEST(Grid2DTest, NodeGetAllNeighbors) {
    Grid2D<int> grid;

    grid.insert(1, 1, 11);
    grid.insert(2, 1, 21);
    grid.insert(1, 2, 12);

    auto* node = grid.getNode(1, 1);
    auto neighbors = node->getAllNeighbors();

    EXPECT_EQ(neighbors.size(), 2);
}

TEST(Grid2DTest, SwapEmptyGrids) {
    Grid2D<int> grid1;
    Grid2D<int> grid2;

    grid1.swap(grid2);

    EXPECT_TRUE(grid1.empty());
    EXPECT_TRUE(grid2.empty());
}

TEST(Grid2DTest, NeighborLinksAfterInsert) {
    Grid2D<int> grid;

    for (int i = 0; i < 3; ++i) {
        grid.insert(i, 0, i);
    }

    auto* node = grid.getNode(1, 0);
    EXPECT_NE(node->getNeighbor(Grid2DNode<int>::Direction::RIGHT), nullptr);
    EXPECT_NE(node->getNeighbor(Grid2DNode<int>::Direction::LEFT), nullptr);
}

TEST(Grid2DTest, IteratorGetXY) {
    Grid2D<int> grid;
    grid.insert(5, 7, 42);

    auto it = grid.begin();
    EXPECT_EQ(it.getX(), 5);
    EXPECT_EQ(it.getY(), 7);
}

TEST(Grid2DTest, IteratorGetNode) {
    Grid2D<int> grid;
    grid.insert(0, 0, 10);

    auto it = grid.begin();
    auto* node = it.getNode();

    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->getData(), 10);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

