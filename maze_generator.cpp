#include <iostream>
#include "maze_generator.h"

maze_generator::maze_generator(int maze_size,       int start_room_size,
                               int quest_room_size, int n_quest_rooms,
                               int n_exits,         Cell start_room_pos) :

                               maze_size(maze_size),
                               start_room_size(start_room_size),
                               quest_room_size(quest_room_size),
                               n_quest_rooms(n_quest_rooms),
                               n_exits(n_exits),
                               start_room_pos(start_room_pos) {};

maze_generator::Path::path_iterator::path_iterator(const Path &p) : p(p) {}

Cell maze_generator::Path::path_iterator::begin(Maze::mask &from, Maze::mask &to) {
    it_pos = 0;
    from = Maze::MAZE;
    to = p.directions[it_pos];
    return p.path[it_pos];
}

Cell maze_generator::Path::path_iterator::next(Maze::mask &from, Maze::mask &to) {
    it_pos++;

    if (it_pos == p.path.size()) return end();
    from = p.directions[it_pos - 1];
    if (it_pos == p.path.size() - 1) to = Maze::MAZE;
    else to = p.directions[it_pos];

    return p.path[it_pos];
}

Cell maze_generator::Path::path_iterator::end() {
    return Cell(-1, -1);
}

void maze_generator::Path::add_cell(Cell c) {
    in_path.insert(c);
    path.push_back(c);
}

void maze_generator::Path::add_cell(Cell c, Maze::mask dir) {
    in_path.insert(c);
    path.push_back(c);
    directions.push_back(dir);
}

void maze_generator::Path::pop_back_until_cell(Cell c) {
    while (path.back() != c) {
        in_path.erase(path.back());
        path.pop_back();
        directions.pop_back();
    }
}

bool maze_generator::Path::contains(Cell c) {
    return (in_path.find(c) != in_path.end());
}

void maze_generator::extract_unvisited_cells(const Maze &maze, std::set<Cell> &unvisited, const Cell &first_visited) {
    Maze::maze_iterator it(maze);
    for (auto i = it.begin(); i != it.end(); i = it.next()) {
        if (!i.in_maze_mask && i != first_visited)
            unvisited.insert(i);
    }
}

Maze::mask maze_generator::invert_direction(Maze::mask m) {
    if (m == Maze::UP || m == Maze::DOWN)
        return m == Maze::UP ? Maze::DOWN : Maze::UP;

    return m == Maze::LEFT ? Maze::RIGHT : Maze::LEFT;
}

void maze_generator::add_room_to_maze_mask(Maze &maze, Cell room_pos, int room_size) {
    Maze::maze_iterator it(maze);
    for (auto i = it.rect_begin(room_pos, room_size); i != it.rect_end(); i = it.rect_next()) {
        maze.apply_mask(i, Maze::MAZE);
    }
}

void maze_generator::add_room_to_room_mask(Maze &maze, Cell room_pos, int room_size, int quest_room_size) {
    Maze::maze_iterator it(maze);
    int mask_x = std::max(room_pos.x - quest_room_size, 0);
    int mask_y = std::max(room_pos.y - quest_room_size, 0);
    int mask_size = quest_room_size + room_size + 1;
    for (auto i = it.rect_begin(Cell(mask_x, mask_y), mask_size);
         i != it.rect_end(); i = it.rect_next()) {
        maze.apply_mask(i, Maze::ROOM);
    }
}

void maze_generator::erase_walls_inside_room(Maze &maze, Cell room_pos, int room_size) {
    Maze::maze_iterator it(maze);
    for (auto i = it.rect_begin(room_pos, room_size); i != it.rect_end(); i = it.rect_next()) {
        int x = i.x - room_pos.x;
        int y = i.y - room_pos.y;

        if (y >= 0 && y < room_size - 1)
            maze.apply_mask(i, Maze::DOWN);
        if (y > 0 && y <= room_size - 1)
            maze.apply_mask(i, Maze::UP);
        if (x >= 0 && x < room_size - 1)
            maze.apply_mask(i, Maze::RIGHT);
        if (x > 0 && x <= room_size - 1)
            maze.apply_mask(i, Maze::LEFT);
    }
}

void maze_generator::open_exit_from_room(Maze &maze, Cell room_pos, int room_size, Maze::mask m) {
    int door_x, door_y;
    int rev_door_x, rev_door_y;
    if (m == Maze::UP) {
        door_x = room_pos.x + room_size / 2;
        door_y = room_pos.y;

        rev_door_x = door_x;
        rev_door_y = door_y - 1;

    } else if (m == Maze::DOWN) {
        door_x = room_pos.x + room_size / 2;
        door_y = room_pos.y + room_size - 1;

        rev_door_x = door_x;
        rev_door_y = door_y + 1;

    } else if (m == Maze::LEFT) {
        door_x = room_pos.x;
        door_y = room_pos.y + room_size  / 2;

        rev_door_x = door_x - 1;
        rev_door_y = door_y;

    } else if (m == Maze::RIGHT) {
        door_x = room_pos.x + room_size - 1;
        door_y = room_pos.y + room_size / 2;

        rev_door_x = door_x + 1;
        rev_door_y = door_y;
    }
    maze.apply_mask(Cell(door_x, door_y), m);
    maze.apply_mask(Cell(rev_door_x, rev_door_y), invert_direction(m));
}

Maze maze_generator::next_maze() {
    Maze maze(maze_size);

    Maze::maze_iterator it(maze);
    // add borders to room mask to avoid generating quest rooms outside the maze
    for (auto i = it.begin(); i != it.end(); i = it.next()) {
        int x = i.x;
        int y = i.y;
        if (y == 0 || (y >= maze_size - quest_room_size && y < maze_size) ||
            x == 0 || (x >= maze_size - quest_room_size && x < maze_size))
            maze.apply_mask(i, Maze::ROOM);
    }

    add_room_to_maze_mask(maze, start_room_pos, start_room_size);
    add_room_to_room_mask(maze, start_room_pos, start_room_size, quest_room_size);
    erase_walls_inside_room(maze, start_room_pos, start_room_size);
    open_exit_from_room(maze, start_room_pos, start_room_size, Maze::DOWN);

    // generate quest rooms
    for (int k = 0; k < n_quest_rooms; ++k) {
        Cell room_pos = maze.next_room();

        add_room_to_maze_mask(maze, room_pos, quest_room_size);
        add_room_to_room_mask(maze, room_pos, quest_room_size, quest_room_size);
        erase_walls_inside_room(maze, room_pos, quest_room_size);

        Maze::mask dir = maze.next_direction();
        open_exit_from_room(maze, room_pos, quest_room_size, dir);
        open_exit_from_room(maze, room_pos, quest_room_size, invert_direction(dir));
    }

    // first visited cell
    Cell first_visited = maze.next_cell();
    // get unvisited cells
    std::set<Cell> unvisited;
    extract_unvisited_cells(maze, unvisited, first_visited);

    // while there are unvisited cells
    while (!unvisited.empty()) {
        Cell cell = maze.next_cell(unvisited);
        Path path;
        path.add_cell(cell);
        // until we hit visited cell
        while (unvisited.find(cell) != unvisited.end()) {
            Maze::mask dir;
            cell = maze.next_cell(cell, dir);
            if (path.contains(cell))
                path.pop_back_until_cell(cell);
            else {
                path.add_cell(cell, dir);
            }
        }
        // carving path
        Path::path_iterator p_it(path);
        Maze::mask from, to;
        for (auto i = p_it.begin(from, to); i != p_it.end(); i = p_it.next(from, to)) {
            if (from == Maze::UP || from == Maze::DOWN || from == Maze::LEFT || from == Maze::RIGHT)
                maze.apply_mask(i, invert_direction(from));
            if (to == Maze::UP || to == Maze::DOWN || to == Maze::LEFT || to == Maze::RIGHT)
                maze.apply_mask(i, to);
            unvisited.erase(i);
        }
    }

    // open exit from maze
    maze.apply_mask(Cell(maze_size / 2, 0), Maze::UP);

    return maze;
}