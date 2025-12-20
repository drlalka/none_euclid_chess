#include "../../include/board/PortalLink.hpp"

PortalLink::PortalLink(int targetSurfaceId, int targetX, int targetY)
    : targetSurfaceId(targetSurfaceId), targetX(targetX), targetY(targetY), active(true) {
}

BoardPosition PortalLink::getDestination() const {
    return BoardPosition(targetSurfaceId, targetX, targetY);
}

bool PortalLink::isActive() const {
    return active;
}

void PortalLink::activate() {
    active = true;
}

void PortalLink::deactivate() {
    active = false;
}

