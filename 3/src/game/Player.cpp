#include "../../include/game/Player.hpp"

Player::Player(const std::string& name, PieceColor color)
    : name(name), color(color) {
}

std::string Player::getName() const {
    return name;
}

PieceColor Player::getColor() const {
    return color;
}

