#include "../../include/common/BoardPosition.hpp"
#include <sstream>
#include <stdexcept>

BoardPosition::BoardPosition() : surfaceId(0), x(0), y(0) {}

BoardPosition::BoardPosition(int sid, int x_, int y_)
    : surfaceId(sid), x(x_), y(y_) {}

bool BoardPosition::equals(const BoardPosition& other) const {
    return surfaceId == other.surfaceId && x == other.x && y == other.y;
}

bool BoardPosition::isValid() const {
    return surfaceId >= 0 && x >= 0 && x < 8 && y >= 0 && y < 8;
}

std::string BoardPosition::toString() const {
    char file = 'a' + x;
    int rank = y + 1;
    std::ostringstream oss;
    oss << surfaceId << ":" << file << rank;
    return oss.str();
}

BoardPosition BoardPosition::fromString(const std::string& str) {
    if (str.length() < 4 || str[1] != ':') {
        throw std::invalid_argument("Invalid position format: " + str);
    }

    int sid = str[0] - '0';
    int x = str[2] - 'a';
    int y = (str[3] - '0') - 1;

    if (str.length() == 5) {
        y = (str[3] - '0') * 10 + (str[4] - '0') - 1;
    }

    return BoardPosition(sid, x, y);
}

bool BoardPosition::operator==(const BoardPosition& other) const {
    return equals(other);
}

bool BoardPosition::operator!=(const BoardPosition& other) const {
    return !equals(other);
}

bool BoardPosition::operator<(const BoardPosition& other) const {
    if (surfaceId != other.surfaceId) return surfaceId < other.surfaceId;
    if (x != other.x) return x < other.x;
    return y < other.y;
}

BoardPosition BoardPosition::operator+(const std::pair<int,int>& offset) const {
    return BoardPosition(surfaceId, x+offset.first, y+offset.second);
}

