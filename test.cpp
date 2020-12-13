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
        dm.depthPath();
        dm.draw();
        dm.removePath();
        dm.breadthPath();
        dm.draw();
        
        PrimMaze pm(&window , 801, 801);
        pm.generate();
        pm.draw();
        pm.depthPath();
        pm.removePath();
        pm.breadthPath();
        pm.draw();

        DivMaze dvm(&window , 801, 801);
        dvm.generate();
        dvm.draw();
        dvm.depthPath();
        dvm.removePath();
        dvm.breadthPath();
        dvm.draw();
        
        RandDivMaze rdm(&window , 801, 801);
        rdm.generate();
        rdm.draw();
        rdm.depthPath();
        rdm.removePath();
        rdm.breadthPath();
        rdm.draw();
    }
    
    return 0;
}   