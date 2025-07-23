@echo off
echo ====================================
echo Building Chess Game in C++
echo ====================================

REM Check if CMake is available
cmake --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: CMake is not installed or not in PATH
    echo Please install CMake from https://cmake.org/download/
    pause
    exit /b 1
)

REM Create build directory
if not exist build mkdir build
cd build

echo.
echo Configuring project with CMake...
cmake .. -DCMAKE_BUILD_TYPE=Release

if errorlevel 1 (
    echo ERROR: CMake configuration failed
    pause
    exit /b 1
)

echo.
echo Building project...
cmake --build . --config Release

if errorlevel 1 (
    echo ERROR: Build failed
    pause
    exit /b 1
)

echo.
echo ====================================
echo Build completed successfully!
echo ====================================
echo.
echo Executable location: build/bin/chess_console.exe
echo.
echo To run the game:
echo   cd build/bin
echo   chess_console.exe
echo.
echo To run tests:
echo   cd build/bin  
echo   chess_test.exe
echo.

pause
