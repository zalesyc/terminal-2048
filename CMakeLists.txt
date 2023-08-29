cmake_minimum_required(VERSION 3.18)
project(terminal-2048)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(terminal-2048 src/main.cpp src/tile.cpp src/move.cpp src/argumentParser.cpp src/game.cpp src/popup.cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

target_link_libraries(terminal-2048 ${CURSES_LIBRARIES})
