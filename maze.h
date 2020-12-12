/**
 * @file maze.h
 * @author Taylor Hallmark
 * @brief header file for generating and solving mazes
 * rendered in sfml
 * @date 12/10
 */

#include <SFML/Graphics.hpp>
#include <stack>
#include <queue>
#include <list>
#include <time.h>
#include <iostream>
#include <unistd.h>

#ifndef MAZE_H
#define MAZE_H

const sf::Color grey(150,150,150);

struct pathNode
{
    pathNode(sf::Vector2i pos, pathNode* par = nullptr)
    {
        positon = pos;
        parent = par;
    }
    sf::Vector2i positon;
    pathNode* parent;
};

class Maze //abstract class for maze sructures
{
public:
    Maze(sf::RenderWindow* w = nullptr,int cols = 0, int rows = 0);
    ~Maze();

    void setRenderSpeed(int s = 0);

    virtual void generate() = 0;
    void depthPath();
    void breadthPath();
    void refresh();
    void reset();
    void draw(int threshold = 0);
    void printGrid(std::ostream &ostr);

protected:
    void initializeSquare(int row, int col);
    bool validMove(sf::Vector2i loc);
    bool isOOB(int row, int col);
    sf::Vector2i betweenSquare(sf::Vector2i a, sf::Vector2i b);

    void drawSquare(char c, float posX, float posY);
    void updateAll(); 

    void drawPath(pathNode* nodePtr);

    int rowSize; //height of maze
    int colSize; //width of maze
    int start; //horizontal position of starting square
    int end; //horizontal position of ending square
    int renderSpeed = 200; //minimum updates before redrawing 
    char** grid; //main maze array

    sf::Vector2f squareSize; //dimensions of each square for rendering
    sf::RenderWindow* window; //pointer to sfml window 
    std::queue<sf::Vector2i> renderQ; //queue of all updated squares to be drawn
};


class DepthMaze : public Maze //depth first search generated maze
{
public:
    DepthMaze(sf::RenderWindow* w,int cols, int rows):
        Maze(w,cols,rows){}

    virtual void generate();
private:
};

class PrimMaze : public Maze //random prims algorithm generated maze
{
public:
    PrimMaze(sf::RenderWindow* w,int cols, int rows):
        Maze(w,cols,rows){}

    virtual void generate();
private:
    void addWalls(int row, int col, std::vector<sf::Vector2i> &w);
    void addCell(int row, int col, std::vector<sf::Vector2i> &w);

};

class TestMaze : public DepthMaze
{
    public:
    TestMaze(sf::RenderWindow* w):
        DepthMaze(w,7,7){}

    virtual void generate();
    void printValidityGrid(std::ostream &ostr);
    void testPath(std::vector<sf::Vector2i> path);
private:
};

#endif