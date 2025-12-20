#ifndef RECTANGULARSURFACE_HPP
#define RECTANGULARSURFACE_HPP

#include "Surface.hpp"

class RectangularSurface : public Surface {
private:
    int width;
    int height;

public:
    RectangularSurface(int surfaceId, int width = 8, int height = 8);

    void initializeBoard() override;
    void setupInitialPosition() override;
    bool isValidCoordinate(int x, int y) const override;
};

#endif // RECTANGULARSURFACE_HPP

