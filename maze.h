#ifndef MAZE_GEN_MAZE_H
#define MAZE_GEN_MAZE_H

//#include <vector>
//#include <set>
//
//#include "cell.h"
//
//class Maze {
//public:
//    void next_maze();
//    std::vector<std::vector<Cell>> get_maze();
//
//private:
//    // constraints
////    const int maze_size = 51;
////    const int start_room_size = 7;
////    const int quest_room_size = 11;
////    const int n_quest_rooms = 3;
////    const int n_exits = 1;
////    const std::pair<int, int> top_left_start_room_corner = {21, 21};
//
//    const int maze_size = 15;
//    const int start_room_size = 3;
//    const int quest_room_size = 4;
//    const int n_quest_rooms = 3;
//    const int n_exits = 1;
//    const std::pair<int, int> start_room_coord = {6, 6}; // i, j
//
//    std::vector<std::vector<Cell>> maze;
//    // mask for generating maze
//    std::vector<std::vector<bool>> maze_mask;
//    // mask for generating quest rooms
//    std::vector<std::vector<bool>> room_mask;
//
//    enum direction { UP, DOWN, LEFT, RIGHT };
//
//
//};

#include <set>
#include <vector>
#include <random>

#include "cell.h"

class Maze {
public:
    Maze(int maze_size);

    class maze_iterator {
    public:
        maze_iterator(const Maze &m);

        Cell begin();
        Cell next();
        Cell end();

        Cell rect_begin(const Cell &c, int size);
        Cell rect_next();
        Cell rect_end();

    private:
        Cell it_pos;

        Cell rect_pos;
        int rect_size;
        int steps_x, steps_y;

        const Maze &m;
    };

    enum mask { MAZE, ROOM, UP, DOWN, LEFT, RIGHT };
    void apply_mask(Cell c, mask);

    Cell next_room();
    Cell next_cell();
    Cell next_cell(Cell cur_cell, Maze::mask &dir);
    Cell next_cell(std::set<Cell> &unvisited);
    Maze::mask next_direction();

private:
    std::mt19937 generator;

    int maze_size;
    std::vector<Cell> maze;
};

#endif //MAZE_GEN_MAZE_H
