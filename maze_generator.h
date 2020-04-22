#ifndef MAZE_GEN_MAZE_GENERATOR_H
#define MAZE_GEN_MAZE_GENERATOR_H

#include <set>

#include "maze.h"

class maze_generator {
public:
    maze_generator(int maze_size,       int start_room_size,
                   int quest_room_size, int n_quest_rooms,
                   int n_exits,         Cell start_room_pos);

    Maze next_maze();

private:
    const int maze_size;
    const int start_room_size;
    const int quest_room_size;
    const int n_quest_rooms;
    const int n_exits;
    const Cell start_room_pos;

    static Maze::mask invert_direction(Maze::mask m);
    static void add_room_to_maze_mask(Maze &maze, Cell room_pos, int room_size);
    static void add_room_to_room_mask(Maze &maze, Cell room_pos, int room_size, int quest_room_size);
    static void erase_walls_inside_room(Maze &maze, Cell room_pos, int room_size);
    static void open_exit_from_room(Maze &maze, Cell room_pos, int room_size, Maze::mask m);

    static void extract_unvisited_cells(const Maze &maze, std::set<Cell> &unvisited, const Cell &first_visited);

    class Path {
    public:
        void add_cell(Cell c);
        void add_cell(Cell c, Maze::mask dir);
        void pop_back_until_cell(Cell c);
        bool contains(Cell c);
        class path_iterator {
        public:
            path_iterator(const Path &p);
            Cell begin(Maze::mask &from, Maze::mask &to);
            Cell next(Maze::mask &from, Maze::mask &to);
            Cell end();

        private:
            int it_pos;
            const Path &p;
        };
    private:
        std::set<Cell> in_path;
        std::vector<Cell> path;
        std::vector<Maze::mask> directions;
    };
};

#endif //MAZE_GEN_MAZE_GENERATOR_H
