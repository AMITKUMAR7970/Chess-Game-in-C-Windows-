#include "../include/Board.h"
#include <iostream>
#include <algorithm>
#include <sstream>

Board::Board() {
    initializeStartingPosition();
}

void Board::initializeStartingPosition() {
    // Clear board
    for (int i = 0; i < 64; i++) {
        state.board[i] = EMPTY;
    }

    // Place pieces
    // White pieces
    state.board[0] = W_ROOK;   state.board[1] = W_KNIGHT; state.board[2] = W_BISHOP; state.board[3] = W_QUEEN;
    state.board[4] = W_KING;   state.board[5] = W_BISHOP; state.board[6] = W_KNIGHT; state.board[7] = W_ROOK;

    for (int i = 8; i < 16; i++) {
        state.board[i] = W_PAWN;
    }

    // Black pieces  
    for (int i = 48; i < 56; i++) {
        state.board[i] = B_PAWN;
    }

    state.board[56] = B_ROOK;  state.board[57] = B_KNIGHT; state.board[58] = B_BISHOP; state.board[59] = B_QUEEN;
    state.board[60] = B_KING;  state.board[61] = B_BISHOP; state.board[62] = B_KNIGHT; state.board[63] = B_ROOK;

    // Initialize game state
    state.currentPlayer = WHITE;
    state.canCastleKingSide[WHITE] = true;
    state.canCastleKingSide[BLACK] = true;
    state.canCastleQueenSide[WHITE] = true;
    state.canCastleQueenSide[BLACK] = true;
    state.enPassantSquare = -1;
    state.halfMoveClock = 0;
    state.fullMoveNumber = 1;

    history.clear();
}

bool Board::isSquareEmpty(int square) const {
    return square >= 0 && square < 64 && state.board[square] == EMPTY;
}

bool Board::isSquareOccupiedBy(int square, Color color) const {
    if (square < 0 || square >= 64) return false;
    Piece piece = state.board[square];
    if (piece == EMPTY) return false;
    return getPieceColor(piece) == color;
}

Color Board::getPieceColor(Piece piece) const {
    if (piece >= W_PAWN && piece <= W_KING) return WHITE;
    if (piece >= B_PAWN && piece <= B_KING) return BLACK;
    return WHITE; // Should not happen for valid pieces
}

bool Board::isInBounds(int rank, int file) const {
    return rank >= 0 && rank < 8 && file >= 0 && file < 8;
}

int Board::getSquare(int rank, int file) const {
    return rank * 8 + file;
}

void Board::getRankFile(int square, int& rank, int& file) const {
    rank = square / 8;
    file = square % 8;
}

bool Board::isPathClear(int from, int to) const {
    int fromRank, fromFile, toRank, toFile;
    getRankFile(from, fromRank, fromFile);
    getRankFile(to, toRank, toFile);

    int rankDir = (toRank > fromRank) ? 1 : (toRank < fromRank) ? -1 : 0;
    int fileDir = (toFile > fromFile) ? 1 : (toFile < fromFile) ? -1 : 0;

    int currentRank = fromRank + rankDir;
    int currentFile = fromFile + fileDir;

    while (currentRank != toRank || currentFile != toFile) {
        if (!isSquareEmpty(getSquare(currentRank, currentFile))) {
            return false;
        }
        currentRank += rankDir;
        currentFile += fileDir;
    }

    return true;
}

bool Board::isValidPawnMove(const Move& move) const {
    int fromRank, fromFile, toRank, toFile;
    getRankFile(move.from, fromRank, fromFile);
    getRankFile(move.to, toRank, toFile);

    Color color = getPieceColor(move.piece);
    int direction = (color == WHITE) ? 1 : -1;
    int startRank = (color == WHITE) ? 1 : 6;

    // Forward move
    if (fromFile == toFile) {
        if (toRank == fromRank + direction && isSquareEmpty(move.to)) {
            return true; // Single step
        }
        if (fromRank == startRank && toRank == fromRank + 2 * direction && 
            isSquareEmpty(move.to) && isSquareEmpty(getSquare(fromRank + direction, fromFile))) {
            return true; // Double step
        }
    }
    // Diagonal capture
    else if (abs(fromFile - toFile) == 1 && toRank == fromRank + direction) {
        if (!isSquareEmpty(move.to) && getPieceColor(state.board[move.to]) != color) {
            return true; // Regular capture
        }
        if (move.to == state.enPassantSquare) {
            return true; // En passant
        }
    }

    return false;
}

bool Board::isValidKnightMove(const Move& move) const {
    int fromRank, fromFile, toRank, toFile;
    getRankFile(move.from, fromRank, fromFile);
    getRankFile(move.to, toRank, toFile);

    int rankDiff = abs(toRank - fromRank);
    int fileDiff = abs(toFile - fromFile);

    return (rankDiff == 2 && fileDiff == 1) || (rankDiff == 1 && fileDiff == 2);
}

bool Board::isValidBishopMove(const Move& move) const {
    int fromRank, fromFile, toRank, toFile;
    getRankFile(move.from, fromRank, fromFile);
    getRankFile(move.to, toRank, toFile);

    int rankDiff = abs(toRank - fromRank);
    int fileDiff = abs(toFile - fromFile);

    return rankDiff == fileDiff && rankDiff > 0 && isPathClear(move.from, move.to);
}

bool Board::isValidRookMove(const Move& move) const {
    int fromRank, fromFile, toRank, toFile;
    getRankFile(move.from, fromRank, fromFile);
    getRankFile(move.to, toRank, toFile);

    return (fromRank == toRank || fromFile == toFile) && 
           fromRank != toRank || fromFile != toFile &&
           isPathClear(move.from, move.to);
}

bool Board::isValidQueenMove(const Move& move) const {
    return isValidRookMove(move) || isValidBishopMove(move);
}

bool Board::isValidKingMove(const Move& move) const {
    int fromRank, fromFile, toRank, toFile;
    getRankFile(move.from, fromRank, fromFile);
    getRankFile(move.to, toRank, toFile);

    int rankDiff = abs(toRank - fromRank);
    int fileDiff = abs(toFile - fromFile);

    // Regular king move
    if (rankDiff <= 1 && fileDiff <= 1 && (rankDiff > 0 || fileDiff > 0)) {
        return true;
    }

    // Castling
    if (move.isCastling && rankDiff == 0 && fileDiff == 2) {
        Color color = getPieceColor(move.piece);
        int rank = (color == WHITE) ? 0 : 7;

        if (fromRank != rank || isInCheck(color)) return false;

        // King side castling
        if (toFile == 6 && state.canCastleKingSide[color]) {
            return isPathClear(move.from, move.to) && 
                   !isSquareAttacked(getSquare(rank, 5), (Color)(1 - color)) &&
                   !isSquareAttacked(getSquare(rank, 6), (Color)(1 - color));
        }
        // Queen side castling  
        if (toFile == 2 && state.canCastleQueenSide[color]) {
            return isPathClear(move.from, getSquare(rank, 1)) &&
                   !isSquareAttacked(getSquare(rank, 3), (Color)(1 - color)) &&
                   !isSquareAttacked(getSquare(rank, 2), (Color)(1 - color));
        }
    }

    return false;
}

bool Board::isSquareAttacked(int square, Color attackingColor) const {
    for (int from = 0; from < 64; from++) {
        Piece piece = state.board[from];
        if (piece == EMPTY || getPieceColor(piece) != attackingColor) continue;

        Move testMove(from, square, piece);

        switch (piece) {
            case W_PAWN:
            case B_PAWN:
                if (isValidPawnMove(testMove)) return true;
                break;
            case W_KNIGHT:
            case B_KNIGHT:
                if (isValidKnightMove(testMove)) return true;
                break;
            case W_BISHOP:
            case B_BISHOP:
                if (isValidBishopMove(testMove)) return true;
                break;
            case W_ROOK:
            case B_ROOK:
                if (isValidRookMove(testMove)) return true;
                break;
            case W_QUEEN:
            case B_QUEEN:
                if (isValidQueenMove(testMove)) return true;
                break;
            case W_KING:
            case B_KING:
                // King attacks like regular move but without castling
                {
                    int fromRank, fromFile, toRank, toFile;
                    getRankFile(from, fromRank, fromFile);
                    getRankFile(square, toRank, toFile);
                    int rankDiff = abs(toRank - fromRank);
                    int fileDiff = abs(toFile - fromFile);
                    if (rankDiff <= 1 && fileDiff <= 1 && (rankDiff > 0 || fileDiff > 0)) {
                        return true;
                    }
                }
                break;
        }
    }
    return false;
}

bool Board::isInCheck(Color color) const {
    // Find king
    Piece king = (color == WHITE) ? W_KING : B_KING;
    for (int i = 0; i < 64; i++) {
        if (state.board[i] == king) {
            return isSquareAttacked(i, (Color)(1 - color));
        }
    }
    return false;
}

bool Board::wouldBeInCheck(const Move& move, Color color) const {
    // Make temporary move
    GameState originalState = state;
    const_cast<Board*>(this)->state.board[move.to] = move.piece;
    const_cast<Board*>(this)->state.board[move.from] = EMPTY;

    bool inCheck = isInCheck(color);

    // Restore state
    const_cast<Board*>(this)->state = originalState;

    return inCheck;
}

std::vector<Move> Board::generateLegalMoves() const {
    std::vector<Move> moves;
    Color currentColor = state.currentPlayer;

    for (int from = 0; from < 64; from++) {
        Piece piece = state.board[from];
        if (piece == EMPTY || getPieceColor(piece) != currentColor) continue;

        for (int to = 0; to < 64; to++) {
            if (from == to) continue;

            Move move(from, to, piece);
            move.captured = state.board[to];

            // Skip if capturing own piece
            if (move.captured != EMPTY && getPieceColor(move.captured) == currentColor) {
                continue;
            }

            // Check basic move validity
            bool valid = false;
            switch (piece) {
                case W_PAWN:
                case B_PAWN:
                    valid = isValidPawnMove(move);
                    if (valid) {
                        // Check for en passant
                        int fromRank, fromFile, toRank, toFile;
                        getRankFile(from, fromRank, fromFile);
                        getRankFile(to, toRank, toFile);
                        if (to == state.enPassantSquare) {
                            move.isEnPassant = true;
                        }
                        // Check for double pawn push
                        if (abs(toRank - fromRank) == 2) {
                            move.isDoublePawnPush = true;
                        }
                        // Check for promotion
                        if ((currentColor == WHITE && toRank == 7) || 
                            (currentColor == BLACK && toRank == 0)) {
                            // Generate all promotion moves
                            Piece promotionPieces[] = {W_QUEEN, W_ROOK, W_BISHOP, W_KNIGHT};
                            if (currentColor == BLACK) {
                                promotionPieces[0] = B_QUEEN; promotionPieces[1] = B_ROOK;
                                promotionPieces[2] = B_BISHOP; promotionPieces[3] = B_KNIGHT;
                            }
                            for (int i = 0; i < 4; i++) {
                                Move promMove = move;
                                promMove.promotion = promotionPieces[i];
                                if (!wouldBeInCheck(promMove, currentColor)) {
                                    moves.push_back(promMove);
                                }
                            }
                            continue;
                        }
                    }
                    break;
                case W_KNIGHT:
                case B_KNIGHT:
                    valid = isValidKnightMove(move);
                    break;
                case W_BISHOP:
                case B_BISHOP:
                    valid = isValidBishopMove(move);
                    break;
                case W_ROOK:
                case B_ROOK:
                    valid = isValidRookMove(move);
                    break;
                case W_QUEEN:
                case B_QUEEN:
                    valid = isValidQueenMove(move);
                    break;
                case W_KING:
                case B_KING:
                    valid = isValidKingMove(move);
                    // Check for castling moves
                    int fromRank, fromFile, toRank, toFile;
                    getRankFile(from, fromRank, fromFile);
                    getRankFile(to, toRank, toFile);
                    if (valid && abs(toFile - fromFile) == 2) {
                        move.isCastling = true;
                    }
                    break;
            }

            if (valid && !wouldBeInCheck(move, currentColor)) {
                moves.push_back(move);
            }
        }
    }

    return moves;
}

bool Board::isValidMove(const Move& move) const {
    std::vector<Move> legalMoves = generateLegalMoves();
    return std::find_if(legalMoves.begin(), legalMoves.end(),
                       [&move](const Move& m) {
                           return m.from == move.from && m.to == move.to &&
                                  m.promotion == move.promotion;
                       }) != legalMoves.end();
}

bool Board::makeMove(const Move& move) {
    if (!isValidMove(move)) {
        return false;
    }

    // Save current state to history
    history.push_back(state);

    // Handle special moves
    if (move.isEnPassant) {
        // Remove captured pawn
        int capturedPawnSquare = state.enPassantSquare + ((state.currentPlayer == WHITE) ? -8 : 8);
        state.board[capturedPawnSquare] = EMPTY;
    }

    if (move.isCastling) {
        // Move rook
        int rank = (state.currentPlayer == WHITE) ? 0 : 7;
        if (move.to % 8 == 6) { // King side
            state.board[getSquare(rank, 5)] = state.board[getSquare(rank, 7)];
            state.board[getSquare(rank, 7)] = EMPTY;
        } else { // Queen side
            state.board[getSquare(rank, 3)] = state.board[getSquare(rank, 0)];
            state.board[getSquare(rank, 0)] = EMPTY;
        }
    }

    // Make the move
    state.board[move.to] = (move.promotion != EMPTY) ? move.promotion : move.piece;
    state.board[move.from] = EMPTY;

    // Update castling rights
    if (move.piece == W_KING || move.from == 4) {
        state.canCastleKingSide[WHITE] = false;
        state.canCastleQueenSide[WHITE] = false;
    }
    if (move.piece == B_KING || move.from == 60) {
        state.canCastleKingSide[BLACK] = false;
        state.canCastleQueenSide[BLACK] = false;
    }
    if (move.from == 0 || move.to == 0) state.canCastleQueenSide[WHITE] = false;
    if (move.from == 7 || move.to == 7) state.canCastleKingSide[WHITE] = false;
    if (move.from == 56 || move.to == 56) state.canCastleQueenSide[BLACK] = false;
    if (move.from == 63 || move.to == 63) state.canCastleKingSide[BLACK] = false;

    // Update en passant
    state.enPassantSquare = -1;
    if (move.isDoublePawnPush) {
        state.enPassantSquare = move.from + ((state.currentPlayer == WHITE) ? 8 : -8);
    }

    // Update move counters
    if (move.piece == W_PAWN || move.piece == B_PAWN || move.captured != EMPTY) {
        state.halfMoveClock = 0;
    } else {
        state.halfMoveClock++;
    }

    if (state.currentPlayer == BLACK) {
        state.fullMoveNumber++;
    }

    // Switch players
    state.currentPlayer = (Color)(1 - state.currentPlayer);

    return true;
}

bool Board::undoMove() {
    if (history.empty()) return false;

    state = history.back();
    history.pop_back();
    return true;
}

bool Board::isCheckmate() const {
    return isInCheck(state.currentPlayer) && generateLegalMoves().empty();
}

bool Board::isStalemate() const {
    return !isInCheck(state.currentPlayer) && generateLegalMoves().empty();
}

bool Board::isDraw() const {
    // 50-move rule
    if (state.halfMoveClock >= 100) return true;

    // Insufficient material (simplified)
    int pieceCount = 0;
    bool hasMinor = false;
    for (int i = 0; i < 64; i++) {
        if (state.board[i] != EMPTY) {
            pieceCount++;
            if (state.board[i] == W_BISHOP || state.board[i] == B_BISHOP ||
                state.board[i] == W_KNIGHT || state.board[i] == B_KNIGHT) {
                hasMinor = true;
            }
        }
    }

    // King vs King, or King+Minor vs King
    if (pieceCount <= 2 || (pieceCount == 3 && hasMinor)) {
        return true;
    }

    return false;
}

std::string Board::squareToAlgebraic(int square) const {
    int rank = square / 8;
    int file = square % 8;
    return std::string(1, 'a' + file) + std::string(1, '1' + rank);
}

int Board::algebraicToSquare(const std::string& algebraic) const {
    if (algebraic.length() != 2) return -1;
    int file = algebraic[0] - 'a';
    int rank = algebraic[1] - '1';
    if (file < 0 || file > 7 || rank < 0 || rank > 7) return -1;
    return rank * 8 + file;
}

void Board::print() const {
    std::cout << "\n  a b c d e f g h\n";
    for (int rank = 7; rank >= 0; rank--) {
        std::cout << (rank + 1) << " ";
        for (int file = 0; file < 8; file++) {
            Piece piece = state.board[rank * 8 + file];
            char symbol = ' ';
            switch (piece) {
                case W_PAWN: symbol = 'P'; break;
                case W_KNIGHT: symbol = 'N'; break;
                case W_BISHOP: symbol = 'B'; break;
                case W_ROOK: symbol = 'R'; break;
                case W_QUEEN: symbol = 'Q'; break;
                case W_KING: symbol = 'K'; break;
                case B_PAWN: symbol = 'p'; break;
                case B_KNIGHT: symbol = 'n'; break;
                case B_BISHOP: symbol = 'b'; break;  
                case B_ROOK: symbol = 'r'; break;
                case B_QUEEN: symbol = 'q'; break;
                case B_KING: symbol = 'k'; break;
                default: symbol = '.'; break;
            }
            std::cout << symbol << " ";
        }
        std::cout << (rank + 1) << "\n";
    }
    std::cout << "  a b c d e f g h\n";
    std::cout << "\nTo move: " << (state.currentPlayer == WHITE ? "White" : "Black") << "\n";
}
