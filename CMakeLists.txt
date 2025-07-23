cmake_minimum_required(VERSION 3.16)
project(ChessGame VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compiler flags for better debugging and warnings
if(MSVC)
    add_compile_options(/W4)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories
include_directories(include)

# Create chess core library
set(CHESS_CORE_SOURCES
    src/core/Board.cpp
    src/core/Game.cpp
)

set(CHESS_CORE_HEADERS
    include/Board.h
    include/Game.h
)

add_library(chesscore STATIC ${CHESS_CORE_SOURCES} ${CHESS_CORE_HEADERS})
target_include_directories(chesscore PUBLIC include)

# Console executable
add_executable(chess_console src/ui/Console.cpp)
target_link_libraries(chess_console chesscore)

# Set the default startup project for Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT chess_console)

# Create a custom target for running the game
add_custom_target(run
    COMMAND chess_console
    DEPENDS chess_console
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Running Chess Game"
)

# Optional: Enable testing
enable_testing()

# Create simple test
add_executable(chess_test tests/basic_test.cpp)
target_link_libraries(chess_test chesscore)
add_test(NAME BasicTest COMMAND chess_test)

# Install targets
install(TARGETS chess_console DESTINATION bin)
install(FILES README.md DESTINATION .)

# CPack configuration for packaging
set(CPACK_PACKAGE_NAME "ChessGame")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Complete Chess Game in C++")
set(CPACK_PACKAGE_VENDOR "Chess Developer")

if(WIN32)
    set(CPACK_GENERATOR "ZIP;NSIS")
else()
    set(CPACK_GENERATOR "TGZ")
endif()

include(CPack)
