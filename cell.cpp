#include "cell.h"

Cell::Cell(int x, int y, bool in_room_mask, bool in_maze_mask, bool up, bool down, bool left, bool right) {
    this->x = x;
    this->y = y;

    this->up = up;
    this->down = down;
    this->left = left;
    this->right = right;

    this->in_room_mask = in_room_mask;
    this->in_maze_mask = in_maze_mask;
}

bool operator != (const Cell &c1, const Cell &c2) {
    return ((c1.x != c2.x) || (c1.y != c2.y) ||
            (c1.up != c2.up) || (c1.down != c2.down) ||
            (c1.left != c2.left) || (c1.right != c2.right) ||
            (c1.in_room_mask != c2.in_room_mask) || (c1.in_maze_mask != c2.in_maze_mask));
}

bool Cell::operator < (const Cell &c) const {
    return x < c.x || (!(c.x < x) && y < c.y);
}