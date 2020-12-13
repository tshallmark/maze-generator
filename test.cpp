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
    sf::RenderWindow window(sf::VideoMode(801,801), "Maze");

    DivMaze m(&window, 10, 10);
    m.generate();
    m.setRenderSpeed(60);

    while(window.isOpen())
    {
        m.draw();
    }

    return 0;
}   