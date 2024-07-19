#pragma once
#include <vector>
#include <memory>
#include <string>
#include "common.h"


//enum class Direction
//{
//	NORTH = 0,
//	SOUTH,
//	EAST,
//	WEST
//};

std::string dirString(Direction dir);


//enum class PathType
//{
//	UNEXPLORED,
//	TERMINATED,
//	CONNECTED
//};

std::string pathTypeString(PathType type);

// Point
//struct Point
//{
//	int x=0;
//	int y=0;
//
//	void print() const;
//	std::string str() const;
//};

class Path;

class Step : Point
{
	Direction dir;
	Direction prevDir;
	//std::unique_ptr<Step> preStep;     // previous and next step info not required, as it can be dealth with Path object
	//std::unique_ptr<Step> nextStep;

public:
	//Step(int X = 0, int Y = 0, Direction prevDir = Direction::EAST, Direction dir = Direction::EAST) :dir{ dir } //, x{X}
	Step(Point p, Direction prevDir = Direction::EAST, Direction dir = Direction::EAST) : Point{ p.x, p.y }, prevDir{ prevDir }, dir { dir } //, x{X}
	{
	}
	Point getPoint();
	Step getNextStep(Direction dir);
	Direction getPrevDir();
	Direction getOppPrevDir();
	void updateDirection(Direction dir);
	void print() const;
	std::string str() const;
};




class Junction : Point
{
	int numOutPaths = 0;  // 2 or 3 are  valid values
	std::vector<Path> outPaths;
	std::vector<Direction> outDirections;
	Direction inputDir;
	Direction selectedOutDir;

public:
	//Junction(Point p, Direction dir);
	Junction(Point p = Point(-1,-1), Direction dir=Direction::INVALID);
	void Add(Path path);
	void setOutDirections(std::vector<Direction> dirs);
	std::vector<Direction> getOutDirections();
	Direction getSelectedOutDirection();
	void setSelectedOutDirection(Direction dir);
	void print() const;
	std::string str() const;
};

class Path
{
	PathType type;
	std::vector<Step> steps;
	Junction endJunction;

public:
	Path(PathType type = PathType::UNEXPLORED);
	PathType getType();
	void setType(PathType type);
	void addStep(Step s);
	void addJunction(Junction j);
	void print() const;
	std::string str() const;
};

class Route
{
	std::vector<Path> paths;
	std::vector<Junction> junctions;

public:
	void AddPath(Path path);
	void AddJunctions(Junction junc);
	void print() const;
	std::string str() const;
};