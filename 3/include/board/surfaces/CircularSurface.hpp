#ifndef CIRCULARSURFACE_HPP
#define CIRCULARSURFACE_HPP

#include "Surface.hpp"

class CircularSurface : public Surface {
private:
    int radius;

public:
    CircularSurface(int surfaceId, int radius);

    void initializeBoard() override;
    void setupInitialPosition() override;
    bool isValidCoordinate(int x, int y) const override;
};

#endif // CIRCULARSURFACE_HPP

