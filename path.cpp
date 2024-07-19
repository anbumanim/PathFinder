#include "path.h"
#include <iostream>
#include <format>

std::string dirString(Direction dir)
{
	std::string str = "Unknown direction";
	switch (dir)
	{
	case Direction::NORTH:
		str = "N"; // "North";
		break;
	case Direction::SOUTH:
		str = "S"; // "South";
		break;
	case Direction::EAST:
		str = "E"; // "East";
		break;
	case Direction::WEST:
		str = "W"; // "West";
		break;
	}
	return str;
}

std::string pathTypeString(PathType type)
{
	std::string str = "Unknown path type";
	switch (type)
	{
	case PathType::UNEXPLORED:
		str = "Unexplored";
		break;
	case PathType::TERMINATED:
		str = "Terminated";
		break;
	case PathType::CONNECTED:
		str = "Connected";
		break;
	}
	return str;
}

std::string Point::str() const
{
	return std::string(std::format("({},{})", x, y));
}

void Point::print() const
{
	std::cout << str();
}

std::string Step::str() const
{
	return std::string(std::format("Step:({},{}){}->{}", x, y, dirString(prevDir), dirString(dir)));
}

Point Step::getPoint()
{
	return Point(x,y);
}

Step Step::getNextStep(Direction dir)
{
	Point p = getPoint();
	if (dir == Direction::NORTH)
	{
		p.y--;
		return Step(p, Direction::SOUTH);
	}
	else if (dir == Direction::SOUTH)
	{
		p.y++;
		return Step(p, Direction::NORTH);
	}
	else if (dir == Direction::EAST)
	{
		p.x++;
		return Step(p, Direction::WEST);
	}
	else if (dir == Direction::WEST)
	{
		p.x--;
		return Step(p, Direction::EAST);
	}
	return Step(p, this->dir);  // Invalid inputs, just send the same step
}

Direction Step::getPrevDir()
{
	return prevDir;
}

Direction Step::getOppPrevDir()
{
	switch (prevDir)
	{
	case Direction::EAST:
		return Direction::WEST;
	case Direction::WEST:
		return Direction::EAST;
	case Direction::NORTH:
		return Direction::SOUTH;
	case Direction::SOUTH:
		return Direction::NORTH;
	}
	return Direction::INVALID;
}

void Step::updateDirection(Direction dir)
{
	this->dir = dir;
}

void Step::print() const
{
	std::cout << str();
}

Path::Path(PathType type) : type{type}
{
}

PathType Path::getType()
{
	return type;
}

void Path::setType(PathType type)
{
	this->type = type;
}

void Path::addStep(Step s)
{
	steps.push_back(s);
}

void Path::addJunction(Junction j)
{
	endJunction = j;
}

std::string Path::str() const
{
	std::string str = std::format("{}->", pathTypeString(type));

	for (const auto s : steps)
	{
		str += s.str();
		str += " ";
	}
	str += ": ";
	return str;
}

void Path::print() const
{
	std::cout << str();
}

Junction::Junction(Point p, Direction dir)
{
	x = p.x;
	y = p.y;
	numOutPaths = 0;
	inputDir = dir;
}

void Junction::Add(Path path)
{
	outPaths.push_back(path);
	numOutPaths++;
}

void Junction::setOutDirections(std::vector<Direction> dirs)
{
	outDirections = dirs;
}

std::vector<Direction> Junction::getOutDirections()
{
	return outDirections;
}

Direction Junction::getSelectedOutDirection()
{
	return selectedOutDir;
}

void Junction::setSelectedOutDirection(Direction dir)
{
	selectedOutDir = dir;
}

void Junction::print() const
{
	std::cout << str();
}

std::string Junction::str() const
{
	std::string str = std::format("Junction: {} outpaths \n", numOutPaths);

	for (const auto path : outPaths)
	{
		str += "\t" + path.str() + "\n";
	}
	return str;
}

void Route::AddPath(Path path)
{
	paths.push_back(path);
}

void Route::AddJunctions(Junction junc)
{
	junctions.push_back(junc);
}
