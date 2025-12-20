#ifndef ENUMS_HPP
#define ENUMS_HPP

enum class GameState {
    NO_GAME,
    NEW_GAME,
    IN_PROGRESS,
    FINISHED
};

enum class GameVerdict {
    CHECK,
    CHECKMATE,
    STALEMATE,
    NONE
};

enum class PieceColor {
    WHITE,
    BLACK
};

enum class SurfaceType {
    RECTANGULAR,
    CIRCULAR,
    CYLINDRICAL,
    SPHERICAL
};

enum class PieceType {
    KING,
    QUEEN,
    BISHOP,
    KNIGHT,
    ROOK,
    PAWN
};

#endif // ENUMS_HPP

