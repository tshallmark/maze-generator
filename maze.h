#include <SFML/Graphics.hpp>
#include <stack>
#include <queue>
#include <time.h>
#include <iostream>

#ifndef MAZE_H
#define MAZE_H

class Maze
{
public:
    Maze(sf::RenderWindow* w = nullptr,int cols = 0, int rows = 0);
    ~Maze();

    void setRenderSpeed(int s = 0);

    virtual void generate() = 0;
    void findPath();
    void refresh();
    void printGrid(std::ostream &ostr);

protected:
    void initializeSquare(int row, int col);
    bool validMove(int row, int col);
    bool isOOB(int row, int col);
    sf::Vector2i betweenSquare(sf::Vector2i a, sf::Vector2i b);

    void draw(int threshold = 0);
    void drawSquare(char c, int posX, int posY);
    void updateAll(); //add every square to renderQ

    int rowSize;
    int colSize;
    int start;
    int end;
    int renderSpeed = 200;
    char** grid;

    sf::Vector2f squareSize;
    sf::RenderWindow* window;
    std::queue<sf::Vector2i> renderQ;
};

class DepthMaze : public Maze
{
public:
    DepthMaze(sf::RenderWindow* w,int cols, int rows):
        Maze(w,cols,rows){}

    virtual void generate();
private:

};

class PrimMaze : public Maze
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