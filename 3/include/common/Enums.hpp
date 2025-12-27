#pragma once

/**
 * @brief Game state enumeration
 */
enum class GameState {
    NO_GAME,      ///< No game initialized
    NEW_GAME,     ///< Game created but not started
    IN_PROGRESS,  ///< Game in progress
    FINISHED      ///< Game ended
};

/**
 * @brief Game verdict enumeration
 */
enum class GameVerdict {
    CHECK,      ///< King is in check
    CHECKMATE,  ///< King is checkmated (game over)
    STALEMATE,  ///< Stalemate (draw)
    NONE        ///< No special state
};

/**
 * @brief Chess piece color
 */
enum class PieceColor {
    WHITE,  ///< White pieces
    BLACK   ///< Black pieces
};

/**
 * @brief Surface topology type
 */
enum class SurfaceType {
    RECTANGULAR,  ///< Flat rectangular board
    CYLINDRICAL,  ///< Cylinder (x-axis wraps)
    SPHERICAL,    ///< Sphere (poles with rotation)
};

/**
 * @brief Chess piece type
 */
enum class PieceType {
    KING,    ///< King piece
    QUEEN,   ///< Queen piece
    BISHOP,  ///< Bishop piece
    KNIGHT,  ///< Knight piece
    ROOK,    ///< Rook piece
    PAWN     ///< Pawn piece
};
