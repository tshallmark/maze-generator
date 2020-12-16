/**
 * @file test.cpp
 * @author Taylor Hallmark
 * @brief testing maze class functions
 * @date 12/10
 */

#include "source/maze.h"
#include "source/mazeClient.h"
#include "iostream"
#include <string>

const int cols = 801;
const int rows = 801;

const int numMazeOptions = 4;
const std::string mazeOptions[] = {
    "Depth first maze", 
    "Prim's algorithim maze",
    "Recursive division maze ", 
    "Random recursive division maze"};

const int numPathOptions = 4;
const std::string pathOptions[] = {
    "Breadth first soulution",
    "Breadth first map",
    "Depth first solution",
    "Depth first map"};

const int numWallOptions = 2;
const std::string wallOptions[] = {
    "Render walls",
    "Color only"};

int main()
{
    sf::RenderWindow window(sf::VideoMode(801,801), "Maze");

    Maze* m = nullptr;
    MazeClient client(&window);
    int mazeSelection;
    int pathSelection;
    int wallSelection;

    while(window.isOpen())
    {
        mazeSelection = client.menu(mazeOptions, numMazeOptions);
        pathSelection = client.menu(pathOptions, numPathOptions);
        wallSelection = client.menu(wallOptions, numWallOptions);
        switch(mazeSelection)
        {
        case 0:
            m  = new DepthMaze(&window, cols, rows);
            break;
        case 1:
            m = new PrimMaze(&window, cols, rows);
            break;
        case 2:
            m = new DivMaze(&window, cols, rows);
            break;
        case 3:
            m = new RandDivMaze(&window, cols, rows);
            break;
        }

        m->setRenderSpeed((cols * rows) / 1000);
        m->generate();
        m->draw();
        
        switch(pathSelection)
        {
        case 0:
            m->bfsPath(1);
            break;
        case 1:
            m->bfsPath(0);
            break;
        case 2:
            m->dfsPath(1);
            break;
        case 3:
            m->dfsPath(0);
            break;
        }

        switch(wallSelection)
        {
        case 0:
            m->setRenderWalls(1);
            break;
        case 1:
            m->setRenderWalls(0);
            break;
        }
        
        m->draw();

        sf::Event event;
        while(window.isOpen())
        {
            if(window.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                {
                    window.close();
                }
                if(event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Enter)
                    {
                        break;
                    }
                }
            }
        }

    }
    return 0;
}   