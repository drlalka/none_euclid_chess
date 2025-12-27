#include <iostream>
#include "../include/game/GameManager.hpp"
#include "../include/game/Player.hpp"
#include "../include/board/surfaces/RectangularSurface.hpp"
#include "../include/pieces/Rook.hpp"
#include "../include/pieces/Pawn.hpp"
#include "../include/pieces/Knight.hpp"

int main() {
    Player player1("White", PieceColor::WHITE);
    Player player2("Black", PieceColor::BLACK);

    GameManager manager;

    auto surface = std::make_unique<RectangularSurface>(0, 8);
    surface->initializeBoard();

    auto whiteRook = std::make_unique<Rook>(PieceColor::WHITE, BoardPosition(0, 0, 0));
    auto whitePawn = std::make_unique<Pawn>(PieceColor::WHITE, BoardPosition(0, 1, 1));
    auto blackPawn = std::make_unique<Pawn>(PieceColor::BLACK, BoardPosition(0, 0, 6));

    surface->addPiece(std::move(whiteRook), 0, 0);
    surface->addPiece(std::move(whitePawn), 1, 1);
    surface->addPiece(std::move(blackPawn), 0, 6);

    manager.gameBoard.addSurface(std::move(surface));
    manager.startNewGame(&player1, &player2);

    std::cout << "\n--- Test 1: Valid Rook Move ---" << std::endl;
    BoardPosition from1(0, 0, 0);
    BoardPosition to1(0, 0, 4);
    Move validMove1(from1, to1);
    bool result1 = manager.validateMove(validMove1);
    std::cout << "Rook 0,0 -> 0,4: " << (result1 ? "VALID ✓" : "INVALID ✗") << std::endl;

    std::cout << "\n--- Test 2: Invalid Rook Diagonal ---" << std::endl;
    BoardPosition from2(0, 0, 0);
    BoardPosition to2(0, 3, 3);
    Move invalidMove2(from2, to2);
    bool result2 = manager.validateMove(invalidMove2);
    std::cout << "Rook 0,0 -> 3,3: " << (result2 ? "VALID ✓" : "INVALID ✗") << std::endl;

    std::cout << "\n--- Test 3: Empty Square ---" << std::endl;
    BoardPosition from3(0, 2, 2);
    BoardPosition to3(0, 2, 3);
    Move invalidMove3(from3, to3);
    bool result3 = manager.validateMove(invalidMove3);
    std::cout << "Empty 2,2 -> 2,3: " << (result3 ? "VALID ✓" : "INVALID ✗") << std::endl;

    std::cout << "\n--- Test 4: Wrong Player (Black piece, White turn) ---" << std::endl;
    BoardPosition from4(0, 0, 6);
    BoardPosition to4(0, 0, 5);
    Move invalidMove4(from4, to4);
    bool result4 = manager.validateMove(invalidMove4);
    std::cout << "Black pawn 0,6 -> 0,5 on White turn: " << (result4 ? "VALID ✓" : "INVALID ✗") << std::endl;

    std::cout << "\n--- Test 5: Valid Pawn Move ---" << std::endl;
    BoardPosition from5(0, 1, 1);
    BoardPosition to5(0, 1, 2);
    Move validMove5(from5, to5);
    bool result5 = manager.validateMove(validMove5);
    std::cout << "Pawn 1,1 -> 1,2: " << (result5 ? "VALID ✓" : "INVALID ✗") << std::endl;

    std::cout << "\n--- Test 6: Same Position Move ---" << std::endl;
    BoardPosition from6(0, 0, 0);
    BoardPosition to6(0, 0, 0);
    Move invalidMove6(from6, to6);
    bool result6 = manager.validateMove(invalidMove6);
    std::cout << "Rook 0,0 -> 0,0: " << (result6 ? "VALID ✓" : "INVALID ✗") << std::endl;

    std::cout << "\n=== Summary ===" << std::endl;
    int passed = 0;
    if (result1 == true) passed++;
    if (result2 == false) passed++;
    if (result3 == false) passed++;
    if (result4 == false) passed++;
    if (result5 == true) passed++;
    if (result6 == false) passed++;

    std::cout << "Tests passed: " << passed << "/6" << std::endl;

    if (passed == 6) {
        std::cout << "✓ All tests PASSED!" << std::endl;
        return 0;
    } else {
        std::cout << "✗ Some tests FAILED!" << std::endl;
        return 1;
    }
}

