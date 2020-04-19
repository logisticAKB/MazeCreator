#include <iostream>

#include "maze.h"
#include "cell.h"

int main() {
    Maze m;
    m.generate_maze();
    std::vector<std::vector<Cell>> maze = m.get_maze();

//    for (int i = 0; i < maze.size(); ++i) {
//        for (int j = 0; j < maze[i].size(); ++j) {
//            std::cout << maze[i][j].up << ' ' << maze[i][j].down << ' '
//                      << maze[i][j].left << ' ' << maze[i][j].right << std::endl;
//        }
//    }

    

    return 0;
}
