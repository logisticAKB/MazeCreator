#ifndef MAZE_GEN_CELL_H
#define MAZE_GEN_CELL_H

class Cell {
public:
    int x;
    int y;

    bool up;
    bool down;
    bool left;
    bool right;

    bool in_room_mask;
    bool in_maze_mask;

    Cell() = default;
    Cell (int x, int y, bool in_room_mask=false, bool in_maze_mask=false,
                        bool up=false,   bool down=false,
                        bool left=false, bool right=false);

    friend bool operator != (const Cell &c1, const Cell &c2);
    bool operator < (const Cell &c) const;
};

#endif //MAZE_GEN_CELL_H
