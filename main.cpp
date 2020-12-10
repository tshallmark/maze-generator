/**
 * @file 
 * @author Taylor Hallmark
 * @brief  
 * @date 
 */

#include "maze.h"
#include "iostream"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1001,1001), "Maze");

    DepthMaze m(&window, 100, 100);
    m.setRenderSpeed(1000);

    m.generate();
    m.findPath();
    m.refresh();

    while(window.isOpen())
    {
        m.draw();
    }

    return 0;
}