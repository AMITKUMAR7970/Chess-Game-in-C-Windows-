# Chess Game - Quick Start Guide

## 🚀 Quick Build & Run (Windows)

### Method 1: Using the build script (Easiest)
1. Double-click `build.bat`
2. Wait for compilation to complete
3. Navigate to `build/bin/` folder
4. Double-click `chess_console.exe`

### Method 2: Manual build
```cmd
mkdir build
cd build
cmake ..
cmake --build . --config Release
cd bin
chess_console.exe
```

## 🎮 Basic Controls

| Command | Description |
|---------|-------------|
| `e2e4` | Move piece from e2 to e4 |
| `help` | Show all commands |
| `moves` | Show legal moves |
| `new` | Start new game |
| `undo` | Undo last move |
| `quit` | Exit game |

## 📋 First Game Example

```
Enter move or command: e2e4      ← Your input
Move played: e2e4

  a b c d e f g h
8 r n b q k b n r 8
7 p p p p p p p p 7
6 . . . . . . . . 6
5 . . . . . . . . 5
4 . . . . P . . . 4
3 . . . . . . . . 3
2 P P P P . P P P 2
1 R N B Q K B N R 1
  a b c d e f g h

To move: Black

Enter move or command: e7e5      ← Your input
```

## 🔧 Troubleshooting

**Build fails?**
- Install Visual Studio with C++ tools
- Install CMake from cmake.org

**Game won't start?**
- Check that `.exe` file exists in `build/bin/`
- Run from command prompt to see error messages

## 📚 More Help

- Read `README.md` for complete documentation
- Type `help` in-game for move formats
- Type `moves` to see all valid moves
