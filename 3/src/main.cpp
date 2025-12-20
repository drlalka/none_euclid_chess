#include <iostream>
#include "../include/game/GameManager.hpp"
#include "../include/game/Player.hpp"
#include "../include/board/surfaces/RectangularSurface.hpp"

int main() {
    std::cout << "Неевклидовы шахматы - Лабораторная работа №3" << std::endl;

    // Создание менеджера игры
    GameManager gameManager;

    // Создание игроков
    Player player1("Игрок 1", PieceColor::WHITE);
    Player player2("Игрок 2", PieceColor::BLACK);

    // TODO: Инициализация игрового поля
    // TODO: Реализация игрового цикла

    std::cout << "Игра завершена" << std::endl;

    return 0;
}

