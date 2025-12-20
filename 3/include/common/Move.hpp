#ifndef MOVE_HPP
#define MOVE_HPP

#include "BoardPosition.hpp"

class Move {
private:
    BoardPosition from;
    BoardPosition to;
    long timestamp;
    bool portalUsed;

public:
    Move(const BoardPosition& from, const BoardPosition& to, bool portalUsed = false);

    const BoardPosition& getFrom() const;
    const BoardPosition& getTo() const;
    long getTimestamp() const;
    bool isPortalUsed() const;
};

#endif // MOVE_HPP

