//
// Created by alexey on 16.04.2020.
//
#include <iostream>
#include <random>
#include <chrono>
#include <map>

#include "maze.h"

void Maze::generate_maze() {
    maze_mask.resize(maze_size, std::vector<bool>(maze_size, false));
    room_mask.resize(maze_size, std::vector<bool>(maze_size, false));

    // rooms can't be generated near borders
    for (int i = 0; i < maze_size; ++i) {
        room_mask[i][0] = true;
        room_mask[i][maze_size - 1] = true;
        room_mask[0][i] = true;
        room_mask[maze_size - 1][i] = true;
    }

    // add start room to maze and room masks
    for (int i = start_room_coord.first; i < start_room_coord.first + start_room_size; ++i) {
        for (int j = start_room_coord.second; j < start_room_coord.second + start_room_size; ++j) {
            maze_mask[i][j] = true;

            room_mask[i][j] = true;
            room_mask[i - 1][j - 1] = true;
            room_mask[i + 1][j + 1] = true;
            room_mask[i - 1][j + 1] = true;
            room_mask[i + 1][j - 1] = true;
        }
    }

//    for (int i = 0; i < maze_size; ++i) {
//        for (int j = 0; j < maze_size; ++j) {
//            std::cout << room_mask[i][j];
//        }
//        std::cout << std::endl;
//    }

    // generate quest rooms and add them to masks
    std::mt19937 g(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> uid(0, maze_size - quest_room_size);

    for (int k = 0; k < n_quest_rooms; ++k) {
        bool invalid_pos = true;
        while (invalid_pos) {
            int room_i = uid(g);
            int room_j = uid(g);
            invalid_pos = false;
            for (int i = room_i; i < room_i + quest_room_size; ++i) {
                for (int j = room_j; j < room_j + quest_room_size; ++j) {
                    if (room_mask[i][j]) invalid_pos = true;
                }
            }
            if (!invalid_pos) {
                for (int i = room_i; i < room_i + quest_room_size; ++i) {
                    for (int j = room_j; j < room_j + quest_room_size; ++j) {
                        maze_mask[i][j] = true;

                        room_mask[i][j] = true;
                        room_mask[i - 1][j - 1] = true;
                        room_mask[i + 1][j + 1] = true;
                        room_mask[i - 1][j + 1] = true;
                        room_mask[i + 1][j - 1] = true;
                    }
                }
            }
        }
    }

    std::cout << std::endl << std::endl;

    for (int i = 0; i < maze_size; ++i) {
        for (int j = 0; j < maze_size; ++j) {
            std::cout << room_mask[i][j];
        }
        std::cout << std::endl;
    }

    std::cout << std::endl << std::endl;

    std::cout << std::endl << std::endl;

    for (int i = 0; i < maze_size; ++i) {
        for (int j = 0; j < maze_size; ++j) {
            std::cout << maze_mask[i][j];
        }
        std::cout << std::endl;
    }

    std::cout << std::endl << std::endl;

    enum direction { UP, DOWN, LEFT, RIGHT };
    maze.resize(maze_size, std::vector<Cell>(maze_size, Cell()));

    // generate maze
    std::set<std::pair<int, int>> unvisited;
    for (int i = 0; i < maze_size; i++) {
        for (int j = 0; j < maze_size; j++) {
            if (!maze_mask[i][j]) unvisited.insert({i, j});
        }
    }

    std::uniform_int_distribution<int> uid_m(0, unvisited.size() - 1);
    int i = start_room_coord.first;
    int j = start_room_coord.second;
    auto rnd_el = unvisited.begin();
    while (maze_mask[i][j]) {
        rnd_el = unvisited.begin();
        int n = uid_m(g);
        std::advance(rnd_el, n);
        i = rnd_el->first;
        j = rnd_el->second;
    }
    unvisited.erase(rnd_el);

    std::cout << maze_size*maze_size-start_room_size*start_room_size-n_quest_rooms*quest_room_size*quest_room_size << std::endl;
    std:: cout << unvisited.size() << std::endl;

    std::set<std::pair<int, int>> in_path;
    std::vector<std::pair<int, int>> path;
    std::vector<direction> path_dir;

    std::uniform_int_distribution<int> dir(0, 3);

    // Wilson
    while (!unvisited.empty()) {
        // random unvisited cell
        std::uniform_int_distribution<int> dist(0, unvisited.size() - 1);
        i = start_room_coord.first;
        j = start_room_coord.second;
        rnd_el = unvisited.begin();
        while (maze_mask[i][j]) {
            rnd_el = unvisited.begin();
            int n = dist(g);
            std::advance(rnd_el, n);
            i = rnd_el->first;
            j = rnd_el->second;
        }
        path.emplace_back(i, j);
        in_path.insert({i, j});
        // random walk until visited cell
        int new_i, new_j;
        direction d;
        do {
            new_i = -1, new_j = -1;
            d = DOWN;
            while (new_i < 0 || new_i >= maze_size
                   || new_j < 0 || new_j >= maze_size || maze_mask[new_i][new_j]) {
                int delta = dir(g);
                switch (delta) {
                    case 0: {
                        new_i = i;
                        new_j = j - 1;
                        d = LEFT;
                        break;
                    }
                    case 2: {
                        new_i = i;
                        new_j = j + 1;
                        d = RIGHT;
                        break;
                    }
                    case 3: {
                        new_i = i - 1;
                        new_j = j;
                        d = UP;
                        break;
                    }
                    default: {
                        new_i = i + 1;
                        new_j = j;
                        d = DOWN;
                        break;
                    }
                }
            }
            // check
            if (in_path.find({new_i, new_j}) != in_path.end()) {
                int cell_i = (path.end() - 1)->first;
                int cell_j = (path.end() - 1)->second;
                while (std::make_pair(cell_i, cell_j) != std::make_pair(new_i, new_j)) {
                    auto b = path.back();
                    path.pop_back();
                    in_path.erase(b);
                    cell_i = (path.end() - 1)->first;
                    cell_j = (path.end() - 1)->second;
                }
                auto b = path.back();
                path.pop_back();
                in_path.erase(b);
            }
            path.emplace_back(new_i, new_j);
            in_path.insert({new_i, new_j});
            path_dir.push_back(d);
            i = new_i;
            j = new_j;
        } while (unvisited.find({new_i, new_j}) != unvisited.end());
        // carve new passages
//        auto b = path.back();
//        path.pop_back();
//        in_path.erase(b);
        std::cout << unvisited.size() << std::endl;
//        for (auto cell : path) {
//            std::cout << cell.first << ' ' << cell.second << " ";
//            unvisited.erase(cell);
//            // add to adj list
//
//        }

        for (int id = 0; id < path.size() - 1; id++) {
            std::pair<int, int> cell = path[id];
            std::pair<int, int> next_cell = path[id + 1];
            unvisited.erase(cell);
            switch (path_dir[id]) {
                case UP: {
                    maze[cell.first][cell.second].up = true;
                    maze[next_cell.first][next_cell.second].down = true;
                    break;
                }
                case DOWN: {
                    maze[cell.first][cell.second].down = true;
                    maze[next_cell.first][next_cell.second].up = true;
                    break;
                }
                case LEFT: {
                    maze[cell.first][cell.second].left = true;
                    maze[next_cell.first][next_cell.second].right = true;
                    break;
                }
                case RIGHT: {
                    maze[cell.first][cell.second].right = true;
                    maze[next_cell.first][next_cell.second].left = true;
                    break;
                }
            }
        }

        std::cout << std::endl << path.size() << ' ' << unvisited.size() << std::endl;
        in_path.clear();
        path.clear();
        path_dir.clear();
    }

    // open passages

}

std::vector<std::vector<Cell>> Maze::get_maze() {
    return maze;
}