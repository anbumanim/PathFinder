#pragma once
#include <vector>
#include "common.h"

//enum class Direction
//{
//	INVALID,
//	NORTH,
//	SOUTH,
//	EAST,
//	WEST
//};

//enum class WayOrWall
//{
//	INVALID,
//	WAY,
//	WALL,
//	ENTRY,
//	EXIT
//};

class Maze
{
	std::vector<std::vector<char>> TwoDMat;
	bool IsMazeValid = false;
	int rows;// = 5;
	int cols;// = 7;
	Point start{ -1, -1 };

	bool validateMaze();
	inline static WayOrWall charToWayWall(char c);


public:
	Maze(const char* fileName);
	inline bool isMazeValid()
	{
		return IsMazeValid;
	}

	WayOrWall getWayWallInfo(Point p, Direction);

	Point getStart();

	//inline static Direction charToDirection(char c);
	inline static  Direction charToDirection(char c)
	{
		Direction dir = Direction::INVALID;

		if (c == 'N') dir = Direction::NORTH;
		if (c == 'S') dir = Direction::SOUTH;
		if (c == 'E') dir = Direction::EAST;
		if (c == 'W') dir = Direction::WEST;

		return dir;
	}
};




/*
MAZE

WWWWWWW  => row 0
WW    W
WWW W W
S   W X
WWWWWWW  => row 4

|     |
v     v

c     c
o     o
l     l

0     6

Direction:

North:   ^ (row-1)
South:   v (row+1)
East:    > (col+1)
West:    < (col-1)

Way = ' '
Wall = 'W'
Entry = 'S'
Exit  = 'X'






*/

