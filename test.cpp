/**
 * @file test.cpp
 * @author Taylor Hallmark
 * @brief testing maze class functions
 * @date 12/10
 */

#include "maze.h"
#include "iostream"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1001,1001), "Maze");

    DepthMaze m(&window, 1001, 1001);
    m.setRenderSpeed(200);
    m.generate();
    m.breadthPath();

    while(window.isOpen())
    {
        m.draw();
    }

    return 0;
}