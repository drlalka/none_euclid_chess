#pragma once

#include <string>
#include "../common/Enums.hpp"
#include "GameHistory.hpp"
#include "ChessClock.hpp"
#include "Player.hpp"
#include "../board/GameBoard.hpp"
#include "../utils/ThreadedVerdictCalculator.hpp"

class Move;
class ChessPiece;

/**
 * @brief Main game manager - controls game state and move validation
 */
class GameManager {
private:
    GameState currentState;
    GameVerdict currentVerdict;
    int currentPlayerId;
    GameHistory gameHistory;
    ChessClock chessClock;
    Player* players[2];
    ThreadedVerdictCalculator verdictCalculator;

    /**
     * @brief Checks if move is legal considering hypothetical check
     * @param move Move to check
     * @return true if king won't be in check after move
     */
    bool isMoveLegalConsideringCheck(const Move& move);

public:
    GameBoard gameBoard;

    GameManager();
    ~GameManager();

    /**
     * @brief Makes a move on the board
     * @param move Move to make
     * @return true if move was successful
     */
    bool makeMove(const Move& move);

    /**
     * @brief Validates move legality
     * @param move Move to validate
     * @return true if move is legal
     */
    bool validateMove(const Move& move);

    /**
     * @brief Calculates game verdict (check/checkmate/stalemate)
     * @return Current game verdict
     */
    GameVerdict calculateVerdict();

    /**
     * @brief Starts new game with two players
     * @param player1 First player
     * @param player2 Second player
     */
    void startNewGame(Player* player1, Player* player2);

    /**
     * @brief Saves game to file
     * @param filename File path
     */
    void saveGame(const std::string& filename);

    /**
     * @brief Loads game from file
     * @param filename File path
     */
    void loadGame(const std::string& filename);

    /**
     * @brief Shows hint based on verdict
     * @param verdict Game verdict
     */
    void showHint(GameVerdict verdict) const;
};

