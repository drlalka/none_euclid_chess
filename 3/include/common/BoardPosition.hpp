#ifndef BOARDPOSITION_HPP
#endif // BOARDPOSITION_HPP

};
    bool equals(const BoardPosition& other) const;
    int getY() const;
    int getX() const;
    int getSurfaceId() const;

    BoardPosition(int surfaceId, int x, int y);
public:

    int y;
    int x;
    int surfaceId;
private:
class BoardPosition {

#define BOARDPOSITION_HPP

