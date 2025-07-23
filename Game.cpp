#include "../include/Game.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

Game::Game() : result(GAME_ONGOING) {
    board.initializeStartingPosition();
}

void Game::newGame() {
    board.initializeStartingPosition();
    moveHistory.clear();
    result = GAME_ONGOING;
}

Move Game::parseMove(const std::string& moveStr) {
    Move move;

    // Handle standard algebraic notation like "e2e4" or "e2-e4"
    std::string cleanStr = moveStr;
    // Remove dashes and spaces
    cleanStr.erase(std::remove(cleanStr.begin(), cleanStr.end(), '-'), cleanStr.end());
    cleanStr.erase(std::remove(cleanStr.begin(), cleanStr.end(), ' '), cleanStr.end());

    if (cleanStr.length() < 4) {
        return move; // Invalid - from and to squares need 4 characters
    }

    // Parse from square
    std::string fromStr = cleanStr.substr(0, 2);
    move.from = board.algebraicToSquare(fromStr);
    if (move.from == -1) return Move(); // Invalid

    // Parse to square  
    std::string toStr = cleanStr.substr(2, 2);
    move.to = board.algebraicToSquare(toStr);
    if (move.to == -1) return Move(); // Invalid

    // Get piece at from square
    move.piece = board.getPiece(move.from);
    if (move.piece == EMPTY) return Move(); // Invalid

    // Get captured piece
    move.captured = board.getPiece(move.to);

    // Handle promotion
    if (cleanStr.length() >= 5) {
        char promotionChar = cleanStr[4];
        Color currentColor = board.getCurrentPlayer();
        switch (promotionChar) {
            case 'Q': case 'q': 
                move.promotion = (currentColor == WHITE) ? W_QUEEN : B_QUEEN; 
                break;
            case 'R': case 'r': 
                move.promotion = (currentColor == WHITE) ? W_ROOK : B_ROOK; 
                break;
            case 'B': case 'b': 
                move.promotion = (currentColor == WHITE) ? W_BISHOP : B_BISHOP; 
                break;
            case 'N': case 'n': 
                move.promotion = (currentColor == WHITE) ? W_KNIGHT : B_KNIGHT; 
                break;
        }
    }

    // Detect special moves based on board state
    const GameState& state = board.getState();

    // Check for castling
    if (move.piece == W_KING || move.piece == B_KING) {
        int fromFile = move.from % 8;
        int toFile = move.to % 8;
        if (abs(toFile - fromFile) == 2) {
            move.isCastling = true;
        }
    }

    // Check for en passant
    if ((move.piece == W_PAWN || move.piece == B_PAWN) && 
        move.to == state.enPassantSquare) {
        move.isEnPassant = true;
    }

    // Check for double pawn push
    if (move.piece == W_PAWN || move.piece == B_PAWN) {
        int fromRank = move.from / 8;
        int toRank = move.to / 8;
        if (abs(toRank - fromRank) == 2) {
            move.isDoublePawnPush = true;
        }
    }

    return move;
}

std::string Game::formatMove(const Move& move) const {
    std::string result = board.squareToAlgebraic(move.from) + 
                        board.squareToAlgebraic(move.to);

    if (move.promotion != EMPTY) {
        char promChar = ' ';
        switch (move.promotion) {
            case W_QUEEN: case B_QUEEN: promChar = 'Q'; break;
            case W_ROOK: case B_ROOK: promChar = 'R'; break;
            case W_BISHOP: case B_BISHOP: promChar = 'B'; break;
            case W_KNIGHT: case B_KNIGHT: promChar = 'N'; break;
        }
        result += promChar;
    }

    return result;
}

void Game::checkGameEnd() {
    if (board.isCheckmate()) {
        result = (board.getCurrentPlayer() == WHITE) ? BLACK_WINS : WHITE_WINS;
    } else if (board.isStalemate() || board.isDraw()) {
        result = DRAW;
    } else {
        result = GAME_ONGOING;
    }
}

bool Game::makeMove(const std::string& moveStr) {
    Move move = parseMove(moveStr);
    return makeMove(move);
}

bool Game::makeMove(const Move& move) {
    if (result != GAME_ONGOING) {
        return false; // Game is over
    }

    if (board.makeMove(move)) {
        moveHistory.push_back(move);
        checkGameEnd();
        return true;
    }

    return false;
}

bool Game::undoLastMove() {
    if (moveHistory.empty()) {
        return false;
    }

    if (board.undoMove()) {
        moveHistory.pop_back();
        result = GAME_ONGOING; // Reset game result
        return true;
    }

    return false;
}

void Game::printBoard() const {
    board.print();
}

void Game::printMoveHistory() const {
    std::cout << "\nMove History:\n";
    for (size_t i = 0; i < moveHistory.size(); i++) {
        if (i % 2 == 0) {
            std::cout << (i / 2 + 1) << ". ";
        }
        std::cout << formatMove(moveHistory[i]);
        if (i % 2 == 0) {
            std::cout << " ";
        } else {
            std::cout << "\n";
        }
    }
    if (moveHistory.size() % 2 == 1) {
        std::cout << "\n";
    }
}

void Game::printGameStatus() const {
    switch (result) {
        case GAME_ONGOING:
            if (board.isInCheck(board.getCurrentPlayer())) {
                std::cout << "\n" << (board.getCurrentPlayer() == WHITE ? "White" : "Black") 
                         << " is in check!\n";
            }
            break;
        case WHITE_WINS:
            std::cout << "\nCheckmate! White wins!\n";
            break;
        case BLACK_WINS:
            std::cout << "\nCheckmate! Black wins!\n";
            break;
        case DRAW:
            if (board.isStalemate()) {
                std::cout << "\nStalemate! The game is a draw.\n";
            } else {
                std::cout << "\nDraw!\n";
            }
            break;
    }
}

std::vector<std::string> Game::getLegalMovesAsStrings() const {
    std::vector<Move> legalMoves = board.generateLegalMoves();
    std::vector<std::string> moveStrings;

    for (const Move& move : legalMoves) {
        moveStrings.push_back(formatMove(move));
    }

    return moveStrings;
}

bool Game::isValidMoveString(const std::string& moveStr) const {
    Move move = const_cast<Game*>(this)->parseMove(moveStr);
    return board.isValidMove(move);
}

bool Game::saveGame(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    // Save move history in simple format
    for (const Move& move : moveHistory) {
        file << formatMove(move) << "\n";
    }

    file.close();
    return true;
}

bool Game::loadGame(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    // Start new game
    newGame();

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            if (!makeMove(line)) {
                // Invalid move found, restore to initial state
                newGame();
                file.close();
                return false;
            }
        }
    }

    file.close();
    return true;
}
