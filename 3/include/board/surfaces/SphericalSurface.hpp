#ifndef SPHERICALSURFACE_HPP
#define SPHERICALSURFACE_HPP

#include "Surface.hpp"

class SphericalSurface : public Surface {
private:
    int radius;

public:
    SphericalSurface(int surfaceId, int radius);

    void initializeBoard() override;
    void setupInitialPosition() override;
    bool isValidCoordinate(int x, int y) const override;
};

#endif // SPHERICALSURFACE_HPP

