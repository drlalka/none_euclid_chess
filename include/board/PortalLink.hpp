#pragma once

#include "../common/BoardPosition.hpp"

/**
 * @brief Represents portal connecting two board positions
 */
class PortalLink {
private:
    BoardPosition targetPosition;
    bool active;

public:
    /**
     * @brief Constructs portal to target position
     * @param target Destination position
     */
    PortalLink(const BoardPosition& target);

    /**
     * @brief Gets destination position
     * @return Target board position
     */
    BoardPosition getDestination() const { return targetPosition; }

    /**
     * @brief Checks if portal is active
     * @return true if portal is enabled
     */
    bool isActive() const { return active; }

    /**
     * @brief Activates portal
     */
    void activate() { active = true; }

    /**
     * @brief Deactivates portal
     */
    void deactivate() { active = false; }
};

