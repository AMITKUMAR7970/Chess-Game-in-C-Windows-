# Chess Game in C++

A complete, fully-featured chess game implementation in C++ designed for Windows 10/11. This project includes all standard chess rules, a console-based interface, and modern C++ design patterns.

## 🎯 Features

### Core Chess Features
- ✅ Complete 8x8 chessboard with all pieces
- ✅ All standard piece movements (Pawn, Knight, Bishop, Rook, Queen, King)
- ✅ Turn-based gameplay (White and Black alternate)
- ✅ Complete move validation system
- ✅ Check, Checkmate, and Stalemate detection
- ✅ Special moves: Castling (King & Queen side)
- ✅ Special moves: En Passant capture
- ✅ Pawn Promotion (to Queen, Rook, Bishop, or Knight)
- ✅ 50-move rule and insufficient material draw detection

### Game Interface
- 🖥️ **Console UI**: Clean ASCII board display with coordinates
- 📝 **Algebraic Notation**: Standard chess move input (e.g., e2e4)
- 🎮 **Interactive Commands**: Help, move history, undo, save/load
- ⏱️ **Move History**: Complete game record with replay capability

### Additional Features
- 💾 **Save/Load Games**: Persistent game storage
- ↩️ **Undo/Redo**: Full move history with unlimited undo
- 🔍 **Legal Move Display**: Show all valid moves for current position
- 🛡️ **Robust Validation**: Comprehensive error checking and input validation

## 🏗️ Project Structure

```
ChessGame/
├── CMakeLists.txt          # Build configuration
├── README.md              # This file  
├── include/               # Public headers
│   ├── Board.h           # Board representation and game state
│   └── Game.h            # Main game controller
├── src/
│   ├── core/             # Chess engine implementation
│   │   ├── Board.cpp     # Board logic and move validation
│   │   └── Game.cpp      # Game controller implementation
│   └── ui/               # User interfaces
│       └── Console.cpp   # Console interface with main()
├── tests/                # Unit tests
│   └── basic_test.cpp    # Basic functionality tests
└── assets/              # Future GUI assets
```

## 🛠️ Build Instructions

### Prerequisites
- **Windows 10/11** (primary target)
- **CMake 3.16+** 
- **C++17 compatible compiler**:
  - Visual Studio 2019/2022 (MSVC) **[Recommended]**
  - MinGW-w64 with GCC 8+
  - Clang 10+

### Option 1: Visual Studio (Recommended)

1. **Install Visual Studio 2019/2022** with C++ development tools
2. **Clone/Download** this project
3. **Open folder** in Visual Studio (File → Open → Folder)
4. **Build** using Ctrl+Shift+B or Build menu
5. **Run** the executable from `build/bin/chess_console.exe`

### Option 2: Command Line Build

```bash
# Navigate to project directory
cd ChessGame

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build . --config Release

# Run the game
./bin/chess_console.exe
```

### Option 3: MinGW/GCC

```bash
# Using MinGW Makefiles
cmake -G "MinGW Makefiles" ..
mingw32-make

# Or using Ninja (if installed)
cmake -G "Ninja" ..
ninja
```

## 🎮 How to Play

### Starting the Game
```
$ ./chess_console.exe
======================================
        CHESS GAME IN C++
======================================

Commands:
  - Make a move: e2e4, a7a5, etc.
  - Castling: e1g1 (king side), e1c1 (queen side)
  - Promotion: e7e8Q (add Q/R/B/N for promotion)
  - 'help' - Show this help
  - 'moves' - Show legal moves
  [... more commands ...]
```

### Making Moves

| Move Type | Format | Example |
|-----------|--------|---------|
| Regular Move | `[from][to]` | `e2e4` |
| Capture | `[from][to]` | `exf5` → `e5f6` |
| Castling King-side | `e1g1` or `e8g8` | `e1g1` |
| Castling Queen-side | `e1c1` or `e8c8` | `e1c1` |
| En Passant | `[from][to]` | `e5f6` |
| Pawn Promotion | `[from][to][piece]` | `e7e8Q` |

### Available Commands

- **`help`** - Display help and move format
- **`moves`** - Show all legal moves for current position
- **`history`** - Display complete move history  
- **`undo`** - Undo the last move
- **`new`** - Start a new game
- **`save <filename>`** - Save current game
- **`load <filename>`** - Load a saved game
- **`quit`** - Exit the game

### Example Game Session
```
  a b c d e f g h
8 r n b q k b n r 8
7 p p p p p p p p 7
6 . . . . . . . . 6
5 . . . . . . . . 5
4 . . . . . . . . 4
3 . . . . . . . . 3
2 P P P P P P P P 2
1 R N B Q K B N R 1
  a b c d e f g h

To move: White

Enter move or command: e2e4

Move played: e2e4
[... game continues ...]
```

## 🧪 Testing

Run the included tests to verify correct installation:

```bash
# Build and run tests
cd build
ctest --output-on-failure

# Or run directly
./bin/chess_test.exe
```

Expected output:
```
Running Chess Game Tests...

✓ Board initialization test passed
✓ Basic move test passed  
✓ Invalid move test passed
✓ Move generation test passed

✅ All tests passed!
```

## 🔧 Advanced Build Options

### Debug Build
```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```

### Release Build (Optimized)
```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

### Create Installation Package
```bash
cmake --build . --target package
```

## 📋 Game Rules Implementation

### Standard Rules ✅
- All pieces move according to FIDE rules
- Proper turn alternation
- Check/Checkmate/Stalemate detection
- Draw by 50-move rule
- Draw by insufficient material
- Three-fold repetition (basic implementation)

### Special Moves ✅
- **Castling**: Both king-side and queen-side
  - King and rook must not have moved
  - No pieces between king and rook
  - King not in check, doesn't pass through check
- **En Passant**: Capture opponent's pawn that moved two squares
- **Pawn Promotion**: Choose Queen, Rook, Bishop, or Knight

## 🐛 Troubleshooting

### Common Build Issues

**CMake not found:**
```bash
# Install CMake from cmake.org or use package manager
winget install Kitware.CMake
```

**Compiler not found:**
```bash
# Ensure Visual Studio C++ tools are installed
# Or install MinGW: winget install mingw
```

**Permission errors:**
- Run command prompt as Administrator
- Check antivirus isn't blocking the executable

### Runtime Issues

**Game crashes on startup:**
- Verify all source files compiled correctly
- Check that assets directory exists (if using GUI version)

**Invalid moves accepted:**
- This indicates a bug - please report with specific position

## 🚀 Future Enhancements

- **GUI Version**: SFML-based graphical interface
- **AI Opponent**: Minimax algorithm with alpha-beta pruning  
- **Network Play**: TCP-based multiplayer
- **Chess Clock**: Timed games with increment support
- **Opening Book**: Database of standard chess openings
- **PGN Export**: Standard chess notation export
- **Chess Engine**: UCI-compatible engine interface

## 📝 License

This project is released under the MIT License. See LICENSE file for details.

## 🤝 Contributing

Contributions welcome! Please feel free to submit pull requests or report issues.

### Code Style
- C++17 standard compliance
- Modern C++ practices (RAII, smart pointers where applicable)
- Clear, self-documenting code with meaningful variable names
- Comprehensive error handling

---

**Enjoy your chess game!** ♟️


