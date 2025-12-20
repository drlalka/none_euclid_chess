#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <string>
#include "../common/Enums.hpp"
#include "GameHistory.hpp"
#include "ChessClock.hpp"
#include "Player.hpp"
#include "../board/GameBoard.hpp"
#include "../utils/ThreadedVerdictCalculator.hpp"

class Move;
class ChessPiece;

class GameManager {
private:
    GameState currentState;
    GameVerdict currentVerdict;
    int currentPlayerId;
    GameHistory gameHistory;
    ChessClock chessClock;
    Player* players[2];
    GameBoard gameBoard;
    ThreadedVerdictCalculator verdictCalculator;
    ChessPiece* capturedFigure;

public:
    GameManager();
    ~GameManager();

    bool makeMove(const Move& move);
    bool validateMove(const Move& move);
    GameVerdict calculateVerdict();
    void startNewGame(Player* player1, Player* player2);
    void saveGame(const std::string& filename);
    void loadGame(const std::string& filename);
    void showHint(GameVerdict verdict);
    ChessPiece* getCapturedFigure() const;
};

#endif // GAMEMANAGER_HPP

