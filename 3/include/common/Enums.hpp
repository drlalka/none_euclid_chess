#pragma once

/**
 * @brief Game state enumeration
 */
enum class GameState {
    NO_GAME,
    NEW_GAME,
    IN_PROGRESS,
    FINISHED
};

/**
 * @brief Game verdict enumeration
 */
enum class GameVerdict {
    CHECK,
    CHECKMATE,
    STALEMATE,
    NONE
};

/**
 * @brief Chess piece color
 */
enum class PieceColor {
    WHITE,
    BLACK
};

/**
 * @brief Surface topology type
 */
enum class SurfaceType {
    RECTANGULAR,
    CYLINDRICAL,
    SPHERICAL
};

/**
 * @brief Chess piece type
 */
enum class PieceType {
    KING,
    QUEEN,
    BISHOP,
    KNIGHT,
    ROOK,
    PAWN
};
