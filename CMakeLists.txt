cmake_minimum_required(VERSION 3.15)
project(maze_gen)

set(CMAKE_CXX_STANDARD 17)

add_executable(maze_gen main.cpp maze.cpp maze.h dsu.cpp dsu.h cell.cpp cell.h)