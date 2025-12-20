#ifndef PORTALLINK_HPP
#define PORTALLINK_HPP

#include "../common/BoardPosition.hpp"

class PortalLink {
private:
    int targetSurfaceId;
    int targetX;
    int targetY;
    bool active;

public:
    PortalLink(int targetSurfaceId, int targetX, int targetY);

    BoardPosition getDestination() const;
    bool isActive() const;
    void activate();
    void deactivate();
};

#endif // PORTALLINK_HPP

