// PathFinder.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "path.h"
#include "Simu.h"

WayOrWall moveOneStep(Step srtStep, Maze& maze, std::vector<Direction>& dirs);
void exploreRoute(Step step, Maze& m, Route routeCur, std::vector<Route>& routes);

void findAllRoutes();



int main()
{
    std::cout << "Path finder!\n";
    Point p;
    p.print();

    Step s1(Point(1,1), Direction::EAST);
    Step s2(Point(2,2), Direction::WEST);
    Step s3(Point(3,3), Direction::NORTH);
    Step s4(Point(4,4), Direction::SOUTH);
    Step s5(Point(5,5), Direction::EAST);
    std::cout << std::endl;

    Path path;
    path.addStep(s1);
    path.addStep(s2);
    path.addStep(s3);
    path.addStep(s4);
    path.addStep(s5);
    path.print();
    std::cout << std::endl;

    Junction j{Point(7,8), Direction::EAST};
    j.Add(path);
    path.setType(PathType::TERMINATED);
    j.Add(path);
    path.setType(PathType::CONNECTED);
    j.Add(path);
    j.print();



    //std::cout << "Path type after one step is " << pathTypeString(pathType) << std::endl;

    findAllRoutes();


    std::cout << "\nPress any key to exit!!!" << std::endl;
    std::cin.ignore();
}

WayOrWall moveOneStep(Step srtStep, Maze& maze, std::vector<Direction>& dirs)
{
    WayOrWall ret = WayOrWall::INVALID;
    int wayCount = 0;
    bool hasExitReached = false;

    Point start(srtStep.getPoint());

    // Except previous direction, check way in other directions
    if ((maze.getWayWallInfo(start, Direction::EAST) == WayOrWall::WAY) && srtStep.getPrevDir() != Direction::EAST)
    {
        dirs.push_back(Direction::EAST);
        wayCount++;
    }

    if ((maze.getWayWallInfo(start, Direction::WEST) == WayOrWall::WAY) && srtStep.getPrevDir() != Direction::WEST)
    {
        dirs.push_back(Direction::WEST);
        wayCount++;
    }
    if((maze.getWayWallInfo(start, Direction::NORTH) == WayOrWall::WAY) && srtStep.getPrevDir() != Direction::NORTH)
    {
        dirs.push_back(Direction::NORTH);
        wayCount++;
    }
    if((maze.getWayWallInfo(start, Direction::SOUTH) == WayOrWall::WAY) && srtStep.getPrevDir() != Direction::SOUTH)
    {
        dirs.push_back(Direction::SOUTH);
        wayCount++;
    }

    // Check if there is an exit present
    if (maze.getWayWallInfo(start, Direction::EAST) == WayOrWall::EXIT)
    {
        dirs.push_back(Direction::EAST);
        hasExitReached = true;
    }

    if (maze.getWayWallInfo(start, Direction::WEST) == WayOrWall::EXIT) 
    {
        dirs.push_back(Direction::WEST);
        hasExitReached = true;
    }
    if (maze.getWayWallInfo(start, Direction::NORTH) == WayOrWall::EXIT)
    {
        dirs.push_back(Direction::NORTH);
        hasExitReached = true;
    }
    if (maze.getWayWallInfo(start, Direction::SOUTH) == WayOrWall::EXIT)
    {
        dirs.push_back(Direction::SOUTH);
        hasExitReached = true;
    }

    switch (wayCount)
    {
    case 0:
        ret = WayOrWall::WALL;
        break;
    case 1:
        ret = WayOrWall::WAY;
        break;
    case 2:
    case 3:
        ret = WayOrWall::JUNCTION;
        break;
    }

    if (hasExitReached)
    {
        ret = WayOrWall::EXIT;
    }

    return ret;
}

void explorePath(Path &path, Step &step, Maze& m, Junction &junc, std::vector<Direction>& dirs)
{
    

    WayOrWall result;
    while (true)
    {
        result = moveOneStep(step, m, dirs);
        std::cout << "Current step is " << step.str() << std::endl;
        if (result == WayOrWall::WAY)
        {
            // update the step with the current direction
            step.updateDirection(dirs[0]);

            // add step to the path
            path.addStep(step);

            // explore on the next step
            step = step.getNextStep(dirs[0]);
            dirs.clear();
        }
        else if (result == WayOrWall::WALL)
        {
            // update the step with the current direction
            step.updateDirection(Direction::INVALID);

            // add step to the path
            path.addStep(step);
            path.setType(PathType::TERMINATED);
            std::cout << "Path terminated at " << step.str() << std::endl;
            break;
        }
        else if (result == WayOrWall::JUNCTION)
        {
            // create junction object and add to the current path
            junc = Junction(step.getPoint(),step.getPrevDir());
            junc.setOutDirections(dirs);
            path.setType(PathType::CONNECTED);
            
            break;
        }
        else if (result == WayOrWall::EXIT)
        {
            // add step to the path
            path.addStep(step);
            path.setType(PathType::EXIT);
            std::cout << "Path exited at " << step.str() << " towards " << dirString(dirs[0]) << " direction." << std::endl;
            break;
        }
    }
}


void findAllRoutes()
{
    std::vector<Route> routes;

    // Create initial path
    Maze m("Maze_5_7.txt");
    std::cout << "is maze valid " << m.isMazeValid() << std::endl;
    std::cout << "Robot is starting at ";
    //m.getStart().print();

    // Start with a route, 
    // continue until a junction found, 
    // then add a new route and continue until exit is found
    Route route;
    Path path{ PathType::EXTRY };
    Point start = m.getStart();
    
    Step srtStep(start, Direction::EAST);  // TBD: find the direction based on the starting point, left, right, top or bottom border of the maze
    path.addStep(srtStep);   // Adding start step to path
    auto step = srtStep.getNextStep(Direction::EAST);
    
    

    exploreRoute(step, m, route, routes);


}

void exploreRoute(Step step, Maze& m, Route routeCur, std::vector<Route>& routes)
{
    std::vector<Direction> dirs;

    while (true)
    {
        Junction junc;
        Path path;     // will it be cleared in next iteration
        dirs.clear();
        explorePath(path, step, m, junc, dirs);

        if (path.getType() == PathType::TERMINATED)
        {
            routeCur.AddPath(path);
            routes.push_back(routeCur);
            return;
        }
        else if (path.getType() == PathType::EXIT)
        {
            std::cout << "Exit reached at " << step.str() << std::endl;
            routeCur.AddPath(path);
            routes.push_back(routeCur);
            return;
        }
        else if (path.getType() == PathType::CONNECTED)
        {
            std::cout << "Junction has reached at " << step.str() << std::endl;
            // complte the path with junction
            routeCur.AddPath(path);

            // Copy the route for another route from the junction
            auto route2 = routeCur;
            auto route3 = routeCur;
            auto step2 = step;
            auto step3 = step;

            // select first direction for the current route and add the junction
            junc.setSelectedOutDirection(dirs[0]);
            routeCur.AddJunctions(junc);

            // pick one direction and continue the route with another path
            // update the step
            step = step.getNextStep(dirs[0]);

            // Initialize for the second route
            junc.setSelectedOutDirection(dirs[1]);
            route2.AddJunctions(junc);
            step2 = step2.getNextStep(dirs[1]);            
            exploreRoute(step2, m, route2, routes);

            // Check if 3rd route is possible and explore it
            if (dirs.size() == 3)
            {
                junc.setSelectedOutDirection(dirs[2]);
                route3.AddJunctions(junc);
                step3 = step3.getNextStep(dirs[2]);
                exploreRoute(step3, m, route3, routes);
            }
        }
    }
    routes.push_back(routeCur);
}





// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
