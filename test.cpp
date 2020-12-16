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

int main()
{
    sf::RenderWindow window(sf::VideoMode(801,801), "Maze");

    DepthMaze m(&window, 20, 20);
    m.generate();
    m.dfsPath(0);
    std::cout << "done\n"; 
    m.draw();

    sf::Event event;
    while(window.isOpen())
    {
        if(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
    }
    return 0;
}   