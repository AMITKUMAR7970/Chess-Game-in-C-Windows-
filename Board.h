#ifndef BOARD_H
#define BOARD_H

#include <cstdint>
#include <vector>
#include <string>

enum Piece {
    EMPTY = 0,
    W_PAWN = 1, W_KNIGHT = 2, W_BISHOP = 3, W_ROOK = 4, W_QUEEN = 5, W_KING = 6,
    B_PAWN = 7, B_KNIGHT = 8, B_BISHOP = 9, B_ROOK = 10, B_QUEEN = 11, B_KING = 12
};

enum Color { WHITE = 0, BLACK = 1 };

struct Move {
    int from;
    int to;
    Piece piece;
    Piece captured;
    Piece promotion;
    bool isEnPassant;
    bool isCastling;
    bool isDoublePawnPush;

    Move() : from(-1), to(-1), piece(EMPTY), captured(EMPTY), 
             promotion(EMPTY), isEnPassant(false), isCastling(false), 
             isDoublePawnPush(false) {}

    Move(int f, int t, Piece p) : from(f), to(t), piece(p), captured(EMPTY),
                                  promotion(EMPTY), isEnPassant(false), 
                                  isCastling(false), isDoublePawnPush(false) {}
};

struct GameState {
    Piece board[64];
    Color currentPlayer;
    bool canCastleKingSide[2];   // [WHITE, BLACK]
    bool canCastleQueenSide[2];  // [WHITE, BLACK]
    int enPassantSquare;         // -1 if no en passant possible
    int halfMoveClock;
    int fullMoveNumber;
};

class Board {
private:
    GameState state;
    std::vector<GameState> history;

    // Helper methods
    bool isSquareEmpty(int square) const;
    bool isSquareOccupiedBy(int square, Color color) const;
    Color getPieceColor(Piece piece) const;
    bool isInBounds(int rank, int file) const;
    int getSquare(int rank, int file) const;
    void getRankFile(int square, int& rank, int& file) const;

    // Move validation helpers
    bool isValidPawnMove(const Move& move) const;
    bool isValidKnightMove(const Move& move) const;
    bool isValidBishopMove(const Move& move) const;
    bool isValidRookMove(const Move& move) const;
    bool isValidQueenMove(const Move& move) const;
    bool isValidKingMove(const Move& move) const;
    bool isPathClear(int from, int to) const;

    // Check detection
    bool isSquareAttacked(int square, Color attackingColor) const;
    bool isInCheck(Color color) const;
    bool wouldBeInCheck(const Move& move, Color color) const;

public:
    Board();
    void initializeStartingPosition();
    void loadFromFEN(const std::string& fen);
    std::string toFEN() const;

    // Game state access
    const GameState& getState() const { return state; }
    Piece getPiece(int square) const { return state.board[square]; }
    Color getCurrentPlayer() const { return state.currentPlayer; }

    // Move operations
    std::vector<Move> generateLegalMoves() const;
    bool isValidMove(const Move& move) const;
    bool makeMove(const Move& move);
    bool undoMove();

    // Game status
    bool isCheckmate() const;
    bool isStalemate() const;
    bool isDraw() const;

    // Display
    void print() const;
    std::string squareToAlgebraic(int square) const;
    int algebraicToSquare(const std::string& algebraic) const;
};

#endif // BOARD_H
