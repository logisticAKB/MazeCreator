cmake_minimum_required(VERSION 3.15)
project(MazeCreator)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_PREFIX_PATH "PATH_TO_QT/QT_VERSION/QT_ARCH/lib/cmake")

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Quick REQUIRED)

add_executable(MazeCreator main.cpp maze.cpp maze.h cell.cpp cell.h maze_generator.cpp maze_generator.h)

target_link_libraries(MazeCreator Qt5::Core)
target_link_libraries(MazeCreator Qt5::Widgets)
target_link_libraries(MazeCreator Qt5::Quick)