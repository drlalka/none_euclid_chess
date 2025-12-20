#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include "../common/Enums.hpp"

class Player {
private:
    std::string name;
    PieceColor color;

public:
    Player(const std::string& name, PieceColor color);

    std::string getName() const;
    PieceColor getColor() const;
};

#endif // PLAYER_HPP

