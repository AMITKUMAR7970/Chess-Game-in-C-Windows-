#include "../include/Game.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

class ConsoleUI {
private:
    Game game;

    void printWelcome() {
        std::cout << "======================================\n";
        std::cout << "        CHESS GAME IN C++             \n";
        std::cout << "======================================\n";
        std::cout << "\nCommands:\n";
        std::cout << "  - Make a move: e2e4, a7a5, etc.\n";
        std::cout << "  - Castling: e1g1 (king side), e1c1 (queen side)\n";
        std::cout << "  - Promotion: e7e8Q (add Q/R/B/N for promotion)\n";
        std::cout << "  - 'help' - Show this help\n";
        std::cout << "  - 'moves' - Show legal moves\n";
        std::cout << "  - 'history' - Show move history\n";
        std::cout << "  - 'undo' - Undo last move\n";
        std::cout << "  - 'new' - Start new game\n";
        std::cout << "  - 'save <filename>' - Save game\n";
        std::cout << "  - 'load <filename>' - Load game\n";
        std::cout << "  - 'quit' - Exit game\n";
        std::cout << "\n";
    }

    void printHelp() {
        std::cout << "\nChess Move Format:\n";
        std::cout << "  Standard: [from][to] (e.g., e2e4)\n";
        std::cout << "  Promotion: [from][to][piece] (e.g., e7e8Q)\n";
        std::cout << "  Piece codes: Q=Queen, R=Rook, B=Bishop, N=Knight\n";
        std::cout << "\nExamples:\n";
        std::cout << "  e2e4     - Pawn from e2 to e4\n";
        std::cout << "  g1f3     - Knight from g1 to f3\n";
        std::cout << "  e1g1     - King-side castling (white)\n";
        std::cout << "  e7e8Q    - Pawn promotion to Queen\n";
        std::cout << "\n";
    }

    void printLegalMoves() {
        std::vector<std::string> moves = game.getLegalMovesAsStrings();
        std::cout << "\nLegal moves (" << moves.size() << " total):\n";

        int count = 0;
        for (const std::string& move : moves) {
            std::cout << move << "  ";
            count++;
            if (count % 8 == 0) {
                std::cout << "\n";
            }
        }
        if (count % 8 != 0) {
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    std::string toLowerCase(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }

    void handleCommand(const std::string& input) {
        std::string command = toLowerCase(input);

        if (command == "help") {
            printHelp();
        } else if (command == "moves") {
            printLegalMoves();
        } else if (command == "history") {
            game.printMoveHistory();
        } else if (command == "undo") {
            if (game.undoLastMove()) {
                std::cout << "\nMove undone.\n";
            } else {
                std::cout << "\nNo moves to undo.\n";
            }
        } else if (command == "new") {
            game.newGame();
            std::cout << "\nNew game started.\n";
        } else if (command.substr(0, 4) == "save") {
            if (command.length() > 5) {
                std::string filename = input.substr(5);
                if (game.saveGame(filename)) {
                    std::cout << "\nGame saved to " << filename << "\n";
                } else {
                    std::cout << "\nFailed to save game.\n";
                }
            } else {
                std::cout << "\nUsage: save <filename>\n";
            }
        } else if (command.substr(0, 4) == "load") {
            if (command.length() > 5) {
                std::string filename = input.substr(5);
                if (game.loadGame(filename)) {
                    std::cout << "\nGame loaded from " << filename << "\n";
                } else {
                    std::cout << "\nFailed to load game.\n";
                }
            } else {
                std::cout << "\nUsage: load <filename>\n";
            }
        } else {
            // Try to parse as a move
            if (game.makeMove(input)) {
                std::cout << "\nMove played: " << input << "\n";
            } else {
                std::cout << "\nInvalid move: " << input << "\n";
                std::cout << "Type 'help' for command list or 'moves' for legal moves.\n";
            }
        }
    }

public:
    void run() {
        printWelcome();

        while (true) {
            // Display current game state
            game.printBoard();
            game.printGameStatus();

            // Check if game is over
            if (game.getResult() != GAME_ONGOING) {
                std::cout << "\nGame over! Type 'new' to start a new game or 'quit' to exit.\n";
            }

            // Get user input
            std::cout << "\nEnter move or command: ";
            std::string input;
            std::getline(std::cin, input);

            // Remove leading/trailing whitespace
            input.erase(0, input.find_first_not_of(" \t"));
            input.erase(input.find_last_not_of(" \t") + 1);

            if (input.empty()) {
                continue;
            }

            if (toLowerCase(input) == "quit" || toLowerCase(input) == "exit") {
                std::cout << "\nThanks for playing!\n";
                break;
            }

            handleCommand(input);
        }
    }
};

// Main function for console version
int main() {
    ConsoleUI ui;
    ui.run();
    return 0;
}
