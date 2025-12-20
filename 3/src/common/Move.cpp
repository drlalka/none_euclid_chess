#include "../../include/common/Move.hpp"
#include <chrono>

Move::Move(const BoardPosition& from, const BoardPosition& to, bool portalUsed)
    : from(from), to(to), portalUsed(portalUsed) {
    timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

const BoardPosition& Move::getFrom() const {
    return from;
}

const BoardPosition& Move::getTo() const {
    return to;
}

long Move::getTimestamp() const {
    return timestamp;
}

bool Move::isPortalUsed() const {
    return portalUsed;
}

