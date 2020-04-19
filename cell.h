//
// Created by alexey on 19.04.2020.
//

#ifndef MAZE_GEN_CELL_H
#define MAZE_GEN_CELL_H

class Cell {
public:
    bool up;
    bool down;
    bool left;
    bool right;

    Cell (bool up=false, bool down=false, bool left=false, bool right=false);
};

#endif //MAZE_GEN_CELL_H
