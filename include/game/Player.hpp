#pragma once

#include <string>
#include "../common/Enums.hpp"

/**
 * @brief Represents a chess player
 */
class Player {
private:
    std::string name;
    PieceColor color;

public:
    /**
     * @brief Constructs player
     * @param name Player name
     * @param color Piece color (WHITE or BLACK)
     */
    Player(const std::string& name, PieceColor color);

    /**
     * @brief Gets player name
     * @return Player name
     */
    std::string getName() const;

    /**
     * @brief Gets player color
     * @return Piece color
     */
    PieceColor getColor() const;
};


