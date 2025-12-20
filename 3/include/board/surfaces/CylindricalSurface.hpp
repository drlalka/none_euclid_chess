#ifndef CYLINDRICALSURFACE_HPP
#define CYLINDRICALSURFACE_HPP

#include "Surface.hpp"

class CylindricalSurface : public Surface {
private:
    int circumference;
    int height;

public:
    CylindricalSurface(int surfaceId, int circumference, int height);

    void initializeBoard() override;
    void setupInitialPosition() override;
    bool isValidCoordinate(int x, int y) const override;
};

#endif // CYLINDRICALSURFACE_HPP

