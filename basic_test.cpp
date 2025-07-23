#include "../include/Board.h"
#include "../include/Game.h"
#include <iostream>
#include <cassert>

void testBoardInitialization() {
    Board board;

    // Test that starting position is correct
    assert(board.getPiece(0) == W_ROOK);
    assert(board.getPiece(1) == W_KNIGHT);
    assert(board.getPiece(4) == W_KING);
    assert(board.getPiece(60) == B_KING);
    assert(board.getCurrentPlayer() == WHITE);

    std::cout << "✓ Board initialization test passed\n";
}

void testBasicMove() {
    Game game;

    // Test valid pawn move
    bool success = game.makeMove("e2e4");
    assert(success);

    // Test that it's now black's turn
    assert(game.getBoard().getCurrentPlayer() == BLACK);

    std::cout << "✓ Basic move test passed\n";
}

void testInvalidMove() {
    Game game;

    // Test invalid move (king jumping)
    bool success = game.makeMove("e1e3");
    assert(!success);

    // Should still be white's turn
    assert(game.getBoard().getCurrentPlayer() == WHITE);

    std::cout << "✓ Invalid move test passed\n";
}

void testMoveGeneration() {
    Board board;

    // At start, white should have 20 possible moves
    std::vector<Move> moves = board.generateLegalMoves();
    assert(moves.size() == 20); // 16 pawn moves + 4 knight moves

    std::cout << "✓ Move generation test passed\n";
}

int main() {
    std::cout << "Running Chess Game Tests...\n\n";

    try {
        testBoardInitialization();
        testBasicMove();
        testInvalidMove();
        testMoveGeneration();

        std::cout << "\n✅ All tests passed!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cout << "\n❌ Test failed: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cout << "\n❌ Unknown test failure\n";
        return 1;
    }
}
