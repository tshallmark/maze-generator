#include "maze.h"
#include "iostream"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1001,1001), "Maze");

    PrimMaze m(&window, 1001, 1001);
    m.setRenderSpeed(1000);

    m.generate();
    m.findPath();
    m.refresh();

    while(window.isOpen())
    {
        
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }
    }

    return 0;
}