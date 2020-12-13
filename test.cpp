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
    
    while(window.isOpen())
    {
        DepthMaze dm(&window , 801, 801);
        dm.generate();
        dm.draw();
        dm.depthPath(0);
        dm.draw();
        dm.removePath();
        dm.breadthPath(0);
        dm.draw();
        
        PrimMaze pm(&window , 801, 801);
        pm.generate();
        pm.draw();
        pm.depthPath(0);
        pm.removePath();
        pm.breadthPath(0);
        pm.draw();

        DivMaze dvm(&window , 801, 801);
        dvm.generate();
        dvm.draw();
        dvm.depthPath(0);
        dvm.removePath();
        dvm.breadthPath(0);
        dvm.draw();
        
        RandDivMaze rdm(&window , 801, 801);
        rdm.generate();
        rdm.draw();
        rdm.depthPath(0);
        rdm.removePath();
        rdm.breadthPath(0);
        rdm.draw();
    }
    
    return 0;
}   