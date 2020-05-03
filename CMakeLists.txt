cmake_minimum_required(VERSION 3.15)
project(maze_gen)

set(CMAKE_CXX_STANDARD 17)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(maze_gen main.cpp maze.cpp maze.h cell.cpp cell.h maze_generator.cpp maze_generator.h)

target_link_libraries( maze_gen ${OpenCV_LIBS} )