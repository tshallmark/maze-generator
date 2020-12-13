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
    DivMaze m(&window, 801, 801);
    m.setRenderSpeed(500);
    m.generate();
    m.breadthPath();
    
    while(window.isOpen())
    {
        m.draw();
    }
    
    return 0;
}   