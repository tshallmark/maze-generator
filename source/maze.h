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

const sf::Color GREY(150,150,150);
const sf::Time MS_PER_FRAME = sf::milliseconds(16);

struct pathNode
{
    pathNode(sf::Vector2i pos, pathNode* par = nullptr)
    {
        positon = pos;
        parent = par;
        if(par!= nullptr)
            distance = par->distance + 1;
    }
    sf::Vector2i positon;
    pathNode* parent;
    int distance=0;
};

struct renderNode
{
    renderNode(sf::Vector2i p, char c = 'a', int v = -1)
    {
        pos = p;
        val = v;
        ch = c;
    }
    sf::Vector2i pos;
    char ch;
    int val;
};

class Maze //abstract class for maze sructures
{
public:
    Maze(sf::RenderWindow* w = nullptr,int cols = 0, int rows = 0);
    ~Maze();

    virtual void generate() = 0;
    void refresh();
    void removePath();
    void draw(bool all = false);
    void printGrid(std::ostream &ostr);
    void randomImperfect(int walls);

    void setRenderSpeed(int s = 0);
    void setRenderWalls(bool r);

    void dfsPath(bool solve = true);
    void bfsPath(bool solve = true);

protected:
    void initializeSquare(int row, int col);
    bool validMove(sf::Vector2i loc, char exception = '?');
    bool isOOB(int row, int col);
    sf::Vector2i betweenSquare(sf::Vector2i a, sf::Vector2i b);

    void renderLoad(sf::Vector2i pos, int val = 0);
    void drawSquare(char c, float posX, float posY, int val);
    void drawLine(sf::Vector2i a, sf::Vector2i b, char c);
    sf::Color getColor(char c, int val);
    void updateAll(); 

    void drawPath(pathNode* nodePtr);

    int rowSize; //height of maze
    int colSize; //width of maze
    int start; //horizontal position of starting square
    int end; //horizontal position of ending square
    int renderSpeed = 50; 
    int maxDistance = 0;
    bool renderWalls = true;
    char** grid; //main maze array

    sf::Vector2f squareSize; //dimensions of each square for rendering
    sf::RenderWindow* window; //pointer to sfml window 
    std::queue<renderNode> renderQ; //queue of all updated squares to be drawn
    
    friend class pathFinder;
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

class DivMaze : public Maze //depth first search generated maze
{
public:
    DivMaze(sf::RenderWindow* w,int cols, int rows):
        Maze(w,cols,rows){}

    virtual void generate();
    void emptyMaze();
protected:
    void recDiv(sf::Vector2i tr, sf::Vector2i bl);
    
    void randHole(sf::Vector2i a, sf::Vector2i b);
};

class RandDivMaze : public DivMaze
{
public:
    RandDivMaze(sf::RenderWindow* w,int cols, int rows):
            DivMaze(w,cols,rows){}
    virtual void generate();
protected:
    virtual void recDiv(sf::Vector2i tr, sf::Vector2i bl);
};
#endif