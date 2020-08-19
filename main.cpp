#include <opencv2/opencv.hpp>

#include "maze_generator.h"

using namespace std;
using namespace cv;

Mat to_image(Maze &maze, int maze_size, int cell_size=10) {
    int n = cell_size * maze_size;
    int sz[] = {n, n};
    Mat img(2, sz, CV_8U, Scalar::all(255));

    Maze::maze_iterator it(maze);
    for (auto i = it.begin(); i != it.end(); i = it.next()) {
        int x1 = i.x * cell_size;
        int y1 = i.y * cell_size;
        int x2 = (i.x + 1) * cell_size;
        int y2 = (i.y + 1) * cell_size;

        if (!i.up) line(img, Point(x1, y1), Point(x2, y1), Scalar::all(0));
        if (!i.left) line(img, Point(x1, y1), Point(x1, y2), Scalar::all(0));
        if (!i.right) line(img, Point(x2, y1), Point(x2, y2), Scalar::all(0));
        if (!i.down) line(img, Point(x1, y2), Point(x2, y2), Scalar::all(0));
    }

    return img;
}

int main() {
    maze_generator m_gen(51, 5, 7, 3, 1, Cell(24, 24));
    auto maze = m_gen.next_maze();
//    imwrite("test.png", to_image(maze, 52));
    namedWindow( "Display window", WINDOW_AUTOSIZE );
    imshow("Display window", to_image(maze, 52));
    waitKey(0);
    return 0;
}
