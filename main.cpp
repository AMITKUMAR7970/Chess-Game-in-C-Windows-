/**
 * Chess Game in C++ - Main Entry Point
 * 
 * This file provides the main entry point for the chess game.
 * The actual UI implementation is in src/ui/Console.cpp
 * 
 * Build: Use CMake to build the project
 * Run: ./chess_console.exe
 */

#include "../include/Game.h"
#include <iostream>

int main() {
    std::cout << "Chess Game in C++\n";
    std::cout << "==================\n\n";

    std::cout << "Note: The actual console interface is in src/ui/Console.cpp\n";
    std::cout << "Use CMake to build and run 'chess_console' target.\n\n";

    // Quick demonstration
    Game game;
    std::cout << "Creating a new chess game...\n";
    game.printBoard();

    std::cout << "\nMaking a test move: e2e4\n";
    if (game.makeMove("e2e4")) {
        std::cout << "Move successful!\n";
        game.printBoard();
    }

    std::cout << "\nFor the full interactive game, build and run 'chess_console'\n";

    return 0;
}
