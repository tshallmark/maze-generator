#include <SFML/Graphics.hpp>
#include <stack>
#include <queue>
#include <time.h>
#include <iostream>

#ifndef MAZE_H
#define MAZE_H

class Maze //abstract class for maze sructures
{
public:
    Maze(sf::RenderWindow* w = nullptr,int cols = 0, int rows = 0);
    ~Maze();

    void setRenderSpeed(int s = 0);

    virtual void generate() = 0;
    void findPath();
    void refresh();
    void draw(int threshold = 0);
    void printGrid(std::ostream &ostr);

protected:
    void initializeSquare(int row, int col);
    bool validMove(int row, int col);
    bool isOOB(int row, int col);
    sf::Vector2i betweenSquare(sf::Vector2i a, sf::Vector2i b);

    void drawSquare(char c, float posX, float posY);
    void updateAll(); 

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

#endif