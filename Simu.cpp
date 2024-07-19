#include "Simu.h"
#include <iostream>
#include <fstream>
#include <string>

Maze::Maze(const char* fileName)
{
    // Open file for read
    std::string line;
    std::ifstream myfile(fileName);

    if (myfile.is_open())
    {
        auto row = 0;
        auto preCol = 0;
        auto curCol = 0;

        while (getline(myfile, line))
        {
            curCol = line.size();
            if (preCol == 0)
            {
                preCol = curCol;
            }
            if (preCol != curCol)
            {
                std::cout << "\n!!! Difference in Columns: row = " << row << ", previous column = " << preCol << ", current column = " << curCol << std::endl;
                TwoDMat.clear();
                row = 0;
                curCol = 0;
                break;
            }
            TwoDMat.push_back(std::vector<char>(line.cbegin(), line.cend()));
            row++;
        }

        rows = row;
        cols = curCol;

        // Validate Maze
        IsMazeValid = validateMaze();
    }
    else {
        std::cout << "Error: Maze file could not be opened!!!" << std::endl;
    }
}

inline bool isMazeValid()
{
    return isMazeValid;
}

inline WayOrWall Maze::charToWayWall(char c)
{
    WayOrWall ret = WayOrWall::INVALID;

    if (c == 'W') ret = WayOrWall::WALL;
    if (c == ' ') ret = WayOrWall::WAY;
    if (c == 'S') ret = WayOrWall::ENTRY;
    if (c == 'X') ret = WayOrWall::EXIT;

    return ret;
}

// inline Direction Maze::charToDirection(char c)
//{
//    Direction dir = Direction::NORTH; // default direction is North
//
//    if (c == 'N') dir = Direction::NORTH;
//    if (c == 'S') dir = Direction::SOUTH;
//    if (c == 'E') dir = Direction::EAST;
//    if (c == 'W') dir = Direction::WEST;
//
//    return dir;
//}

WayOrWall Maze::getWayWallInfo(Point pos, Direction dir)
{
    WayOrWall ret = WayOrWall::INVALID;
    auto x = pos.y;    // TBD : Need major modification
    auto y = pos.x;

    // Check x and y are in valid range 1 < row < rows - 1, 1 < col < cols - 1
    if ((0 < x && x < rows - 1) && (0 < y && y < cols - 1))
    {
        // check if the current position is wall
        if (TwoDMat[x][y] == 'W')
        {
            std::cout << "Current position (" << x << "," << y << ") is a WALL!!" << std::endl;
            return ret;
        }
        switch (dir) {
        case Direction::NORTH:
            ret = charToWayWall(TwoDMat[x - 1][y]);
            break;
        case Direction::SOUTH:
            ret = charToWayWall(TwoDMat[x + 1][y]);
            break;
        case Direction::EAST:
            ret = charToWayWall(TwoDMat[x][y + 1]);
            break;
        case Direction::WEST:
            ret = charToWayWall(TwoDMat[x][y - 1]);
            break;
        }
    }
    else
    {
        std::cout << "(" << x << ", " << y << ") is out of bondary of the maze" << std::endl;
        ret = WayOrWall::INVALID;
    }
    return ret;
}

Point Maze::getStart()
{
    return start;
}

//char TwoDMat[7][5];

bool Maze::validateMaze()
{
    //std::cout << "Validating maze ..." << std::endl;
    // Check boundary
    int entryCnt = 0;
    int exitCnt = 0;
    bool ret = true;

    // Iterate top and bottom rows
    for (int col = 0; col < cols; col++)
    {
        if (TwoDMat[0][col] == 'W');
        else if (TwoDMat[0][col] == 'S') {
            start.x = col; start.y = 0;
            entryCnt++;
        }
        else if (TwoDMat[0][col] == 'X') {
            exitCnt++;
        }
        else
        {
            ret = false;
            std::cout << "Invalid value found in the TOP row of the maze at col" << col << " and the value is " << TwoDMat[0][col] << std::endl;
        }
        if (TwoDMat[rows - 1][col] == 'W');
        else if (TwoDMat[rows - 1][col] == 'S') {
            start.x = col; start.y = rows-1;
            entryCnt++;
            continue;
        }
        else if (TwoDMat[rows - 1][col] == 'X') {
            exitCnt++;
            continue;
        }
        else
        {
            ret = false;
            std::cout << "Invalid value found in the BOTTOM row of the maze at col" << col << " and the value is " << TwoDMat[rows - 1][col] << std::endl;
        }
    }

    // Iterate left and right columns
    for (int row = 0; row < rows; row++)
    {
        if (TwoDMat[row][0] == 'W');
        else if (TwoDMat[row][0] == 'S') {
            start.x = 0; start.y = row;
            entryCnt++;
        }
        else if (TwoDMat[row][0] == 'X') {
            exitCnt++;
        }
        else
        {
            ret = false;
            std::cout << "Invalid value found in the LEFT of the maze at row" << row << " and the value is " << TwoDMat[row][0] << std::endl;
        }

        if (TwoDMat[row][cols - 1] == 'W')
            ;
        else if (TwoDMat[row][cols - 1] == 'S') {
            start.x = cols-1; start.y = row;
            entryCnt++;
        }
        else if (TwoDMat[row][cols - 1] == 'X') {
            exitCnt++;
        }
        else
        {
            ret = false;
            std::cout << "Invalid value found in the RIGHT of the maze at row" << row << " and the value is " << TwoDMat[row][cols - 1] << std::endl;
        }
    }
    if (entryCnt == 0) {
        std::cout << "No entry is found in the boundary. ";
        ret = false;
    }
    if (entryCnt > 1) {
        std::cout << "There are more than one entry [" << entryCnt << "] found in the boundary. ";
        ret = false;
    }
    if (exitCnt == 0) {
        std::cout << "No exit is found in the boundary. ";
        ret = false;
    }
    if (exitCnt > 1) {
        std::cout << "There are more than one exit [" << exitCnt << "] found in the boundary. ";
        ret = false;
    }

    // Iterate internal of the maze
    for (int row = 1; row < rows - 1 && ret; row++)
    {
        for (int col = 1; col < cols - 1; col++)
        {
            // Check if the char is 'W' or ' ' then continue; otherwise return -1
            if (TwoDMat[row][col] == 'W' || TwoDMat[row][col] == ' ') continue;
            else
            {
                std::cout << "Internal value of the maze is incorrect at [" << row << "," << col << "] is " << TwoDMat[row][col] << std::endl;
                ret = false;
                break;
            }
        }
    }
    return ret;
}