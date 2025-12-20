#include <iostream>
#include "../include/container/Grid2D.hpp"

int main() {
    std::cout << "=== Пример использования Grid2D ===" << std::endl;
    std::cout << std::endl;

    Grid2D<int> grid;

    std::cout << "Создание сетки 3x3..." << std::endl;
    for (int x = 0; x < 3; ++x) {
        for (int y = 0; y < 3; ++y) {
            grid.insert(x, y, x * 10 + y);
        }
    }

    std::cout << "Размер сетки: " << grid.size() << std::endl;
    std::cout << std::endl;

    std::cout << "Элементы сетки:" << std::endl;
    for (int y = 2; y >= 0; --y) {
        for (int x = 0; x < 3; ++x) {
            std::cout << "[" << x << "," << y << "]=" << grid.at(x, y) << "  ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Соседи узла (1,1):" << std::endl;
    auto* centerNode = grid.getNode(1, 1);
    if (centerNode) {
        using Dir = Grid2DNode<int>::Direction;

        auto printNeighbor = [&](Dir dir, const char* name) {
            auto* neighbor = centerNode->getNeighbor(dir);
            if (neighbor) {
                std::cout << "  " << name << ": [" << neighbor->getX() << ","
                         << neighbor->getY() << "] = " << neighbor->getData() << std::endl;
            }
        };

        printNeighbor(Dir::RIGHT, "RIGHT     ");
        printNeighbor(Dir::RIGHT_UP, "RIGHT_UP  ");
        printNeighbor(Dir::UP, "UP        ");
        printNeighbor(Dir::LEFT_UP, "LEFT_UP   ");
        printNeighbor(Dir::LEFT, "LEFT      ");
        printNeighbor(Dir::LEFT_DOWN, "LEFT_DOWN ");
        printNeighbor(Dir::DOWN, "DOWN      ");
        printNeighbor(Dir::RIGHT_DOWN, "RIGHT_DOWN");
    }
    std::cout << std::endl;

    std::cout << "Итерация по всем элементам:" << std::endl;
    int sum = 0;
    for (const auto& value : grid) {
        sum += value;
    }
    std::cout << "Сумма всех элементов: " << sum << std::endl;
    std::cout << std::endl;

    std::cout << "Удаление элемента (1,1)..." << std::endl;
    grid.erase(1, 1);
    std::cout << "Новый размер: " << grid.size() << std::endl;
    std::cout << "Элемент (1,1) существует? " << (grid.contains(1, 1) ? "Да" : "Нет") << std::endl;
    std::cout << std::endl;

    struct Point {
        int x, y;
        Point(int x = 0, int y = 0) : x(x), y(y) {}
    };

    Grid2D<Point> pointGrid;
    pointGrid.insert(0, 0, Point(10, 20));
    pointGrid.insert(1, 0, Point(30, 40));

    std::cout << "Сетка точек:" << std::endl;
    std::cout << "  (0,0): Point(" << pointGrid.at(0, 0).x << ", "
              << pointGrid.at(0, 0).y << ")" << std::endl;
    std::cout << "  (1,0): Point(" << pointGrid.at(1, 0).x << ", "
              << pointGrid.at(1, 0).y << ")" << std::endl;

    return 0;
}

