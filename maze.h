//
// Created by alexey on 16.04.2020.
//

#ifndef MAZE_GEN_MAZE_H
#define MAZE_GEN_MAZE_H

#include <vector>
#include <set>

#include "cell.h"

class Maze {
public:
    void generate_maze();
    std::vector<std::vector<Cell>> get_maze();

private:
    // constraints
//    const int maze_size = 51;
//    const int start_room_size = 7;
//    const int quest_room_size = 11;
//    const int n_quest_rooms = 3;
//    const int n_exits = 1;
//    const std::pair<int, int> top_left_start_room_corner = {21, 21};

    const int maze_size = 15;
    const int start_room_size = 3;
    const int quest_room_size = 4;
    const int n_quest_rooms = 3;
    const int n_exits = 1;
    const std::pair<int, int> start_room_coord = {6, 6}; // i, j

    std::vector<std::vector<Cell>> maze;
    // mask for generating maze
    std::vector<std::vector<bool>> maze_mask;
    // mask for generating quest rooms
    std::vector<std::vector<bool>> room_mask;
};

#endif //MAZE_GEN_MAZE_H
