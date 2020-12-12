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
    sf::RenderWindow window(sf::VideoMode(800,800), "Maze");

    DepthMaze m(&window, 100, 100);
    m.setRenderSpeed(50);
    m.generate();

    while(window.isOpen())
    {
        m.depthPath();
        m.reset();
        m.breadthPath();
        m.reset();
        m.draw();
    }

    return 0;
}