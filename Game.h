#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include <string>
#include <vector>

enum GameResult {
    GAME_ONGOING,
    WHITE_WINS,
    BLACK_WINS, 
    DRAW
};

class Game {
private:
    Board board;
    std::vector<Move> moveHistory;
    GameResult result;

    // Helper methods
    Move parseMove(const std::string& moveStr);
    std::string formatMove(const Move& move) const;
    void checkGameEnd();

public:
    Game();

    // Game control
    void newGame();
    bool makeMove(const std::string& moveStr);
    bool makeMove(const Move& move);
    bool undoLastMove();

    // Game state access
    const Board& getBoard() const { return board; }
    GameResult getResult() const { return result; }
    const std::vector<Move>& getMoveHistory() const { return moveHistory; }

    // Display
    void printBoard() const;
    void printMoveHistory() const;
    void printGameStatus() const;

    // Save/Load
    bool saveGame(const std::string& filename) const;
    bool loadGame(const std::string& filename);

    // Utilities
    std::vector<std::string> getLegalMovesAsStrings() const;
    bool isValidMoveString(const std::string& moveStr) const;
};

#endif // GAME_H
