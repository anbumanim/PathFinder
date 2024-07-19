#pragma once
#include <string>



enum class Direction
{
	INVALID = -1,
	NORTH = 0,
	SOUTH,
	EAST,
	WEST
};

//namespace Direction
//{
//	enum Type
//	{
//		INVALID = -1,
//		NORTH = 0,
//		SOUTH,
//		EAST,
//		WEST
//	};
//	static const Type All[] = {INVALID, NORTH, SOUTH, EAST, WEST};
//};


enum class PathType
{
	UNEXPLORED,
	TERMINATED,
	CONNECTED,
	EXTRY,
	EXIT
};

enum class WayOrWall
{
	INVALID,
	WAY,
	WALL,
	JUNCTION,
	ENTRY,
	EXIT
};

// Point
struct Point
{
	int x = 0;
	int y = 0;

	void print() const;
	std::string str() const;
};


