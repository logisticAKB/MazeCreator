#include <iostream>

#include "maze_generator.h"

int main() {
    maze_generator m_gen(15, 3, 3, 3, 1, Cell(6, 6));
    auto maze = m_gen.next_maze();

    Maze::maze_iterator it(maze);
//    for (auto i = it.rect_begin(Cell(6, 6), 3); i != it.rect_end(); i = it.rect_next()) {
//        std::cout << i.y << ' ' << i.x << ' ' << i.up << ' ' << i.down << ' ' << i.left << ' ' << i.right << std::endl;
////        std::cout << i.y << ' ' << i.x << ' ' << i.in_room_mask << std::endl;
//    }



    return 0;
}
