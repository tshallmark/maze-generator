#include "maze.h"

Maze::Maze(sf::RenderWindow* w,int cols, int rows)
{
    srand (time(NULL));

    //verify dimensions are odd
    if(rows % 2 == 0)
        rows--;
    if(cols % 2 == 0)
        cols--;

    rowSize = rows;
    colSize = cols;
    window = w;

    //calculate dimensions of each cell for rendering
    squareSize.x = window->getSize().x / (float)colSize;
    squareSize.y = window->getSize().y / (float)rowSize;

    grid = new char*[rowSize];
    for(int row = 0; row < rowSize; row++)
    {
        grid[row] = new char[colSize];
        for (size_t col = 0; col < colSize; col++)
        {
            initializeSquare(row,col);
            //push each square to render queue
            //order reversed for x/y coordinates
            renderQ.push(sf::Vector2i(col,row));
        }
    }

    //place start & end on random square of top & bottom row
    start = 1 + 2 * (rand() % (colSize / 2));
    end = 1 + 2 * (rand() % (colSize / 2));

    grid[0][start] = 's';
    grid[rowSize-1][end] = 'e';
}

Maze::~Maze()
{
    //destroy each subArray
    for(int i = 0; i < rowSize; i++)
    {
        delete [] grid[i];
        grid[i] = nullptr;
    }
    //destroy primary array
    delete grid;
    grid = nullptr;
}

void Maze::setRenderSpeed(int s)
{
    //validate speed
    if(s >= 0)
        renderSpeed = s;
}

void Maze::initializeSquare(int row, int col)
{
    if((row > 0 && row < rowSize - 1) //if not on perimeter
    && (col > 0 && col < colSize - 1)
    && (row % 2 == 1) && (col % 2 == 1)) //and on odd grid aquare
    {
        grid[row][col] = '-'; //mark grid square as empty
    }else
    {
        grid[row][col] = '#'; //otherwise mark grid square as wall
    }
}

void Maze::findPath()
{
    int maxDepth = 0;
    std::stack<sf::Vector2i> path;
    sf::Vector2i curr(start, 0);
    path.push(curr);

    while(!path.empty())
    {
        if(path.size() > maxDepth)
            maxDepth = path.size();
        // std::cout << "current square: [" << curr.x << "][" << curr.y << "]\n" \
        //           << "stack depth: " << path.size() << '\n'
        //           << "max Depth: " << maxDepth << "\n\n";
        
        curr = path.top();
        path.pop();

        if(grid[curr.y][curr.x] == 'e')
            break;

        grid[curr.y][curr.x] = 'p';

        if(validMove(curr.y-1,curr.x) ) //check north
        {
            path.push(curr);
            path.push(sf::Vector2i(curr.x, curr.y-1));
            
        }else if( validMove(curr.y+1,curr.x) ) //check south
        {
            path.push(curr);
            path.push(sf::Vector2i(curr.x, curr.y+1));
        }else if( validMove(curr.y,curr.x+1) ) //check east
        {
            path.push(curr);
            path.push(sf::Vector2i(curr.x+1, curr.y));
        }else if( validMove(curr.y,curr.x-1) ) //check west
        {
            path.push(curr);
            path.push(sf::Vector2i(curr.x-1,curr.y));
        }else
        {
            grid[curr.y][curr.x] = 'f';
        }
        renderQ.push(curr);
        draw(renderSpeed);
    }
}


bool Maze::validMove(int row, int col)
{
    //return true if
    return ( !isOOB(row, col) //not out of bounds
        && grid[row][col] != '#' // and not a wall 
        && grid[row][col] != 'p' // and not on current path
        && grid[row][col] != 'f');// and not on failed path
}

bool Maze::isOOB(int row, int col)
{
    if(col < 0 || col >= colSize || row < 0 || row >= rowSize)
        return true;
    return false;
}

sf::Vector2i Maze::betweenSquare(sf::Vector2i a,sf::Vector2i b)
{
    //set between coordinates to half the difference of a & b coordinates
    sf::Vector2i between( (b.x - a.x)/2, (b.y - a.y)/2 );

    //add between vector back to a
    between.x += a.x;
    between.y += a.y;

    return between;
}

void Maze::refresh()
{
    updateAll();
    draw();
}

void Maze::updateAll()
{
    for(size_t row = 0; row < rowSize; row++)
    {
        for (size_t col = 0; col < colSize; col++)
        {
            renderQ.push(sf::Vector2i(col, row));
        }
    }
}

void Maze::draw(int threshold)
{
    //only render if queue is larger than threshold 
    if(renderQ.size() > threshold)
    {
        //render squares until queue is empty
        while(!renderQ.empty())
        {
            //get + pop next square
            sf::Vector2i temp = renderQ.front();
            renderQ.pop();
            //find window coordinates of square
            int posX = 0 + (temp.x * squareSize.x);
            int posY = 0 + (temp.y * squareSize.y);
            
            drawSquare(grid[temp.y][temp.x], posX, posY);
        }
        window->display();
    }
}

void Maze::drawSquare(char c, int posX, int posY)
{
    //create and place shape
    sf::RectangleShape square(squareSize);
    square.setPosition(posX, posY);

    if(c == '#' || c == '-') //color walls black
        square.setFillColor(sf::Color::Black);
    else if (c == 'o')
    {
        square.setFillColor(sf::Color::White);
    }else if (c == 'f')
    {
        square.setFillColor(sf::Color::Cyan);
    }else if (c == 'p')
    {
        square.setFillColor(sf::Color::Blue);
    }else if (c == 's')
    {
        square.setFillColor(sf::Color::Green);
    }else if (c =='e')
    {
        square.setFillColor(sf::Color::Red);
    }
    
    window ->draw(square);
}

void Maze::printGrid(std::ostream &ostr)
{
    std::cout << ' ';
    for (size_t i = 0; i < colSize ; i++)
    {
        std::cout << i;
    }
    std::cout << '\n';
    
    for (size_t row = 0; row < rowSize; row++)
    {
        std::cout << row;
        for (size_t col = 0; col < colSize; col++)
        {
            ostr << grid[row][col];
        }
        ostr << "\n";
    }
    ostr << "\n";
}



void DepthMaze::generate()
{
    int maxDepth = 0;
    int row = 1 + 2 * (rand() % (rowSize / 2));
    int col = 1 + 2 * (rand() % (colSize / 2));

    std::stack<sf::Vector2i> path;

    sf::Vector2i curr(col, row);
    grid[row][col] = 'o';
    renderQ.push(curr);
    path.push(curr);

    while(!path.empty())
    {
        if(path.size() > maxDepth)
            maxDepth = path.size();
            
        curr = path.top();
        path.pop();
        //array of coordinates of adjacent squares
        sf::Vector2i neighbors[4];
        neighbors[0] = sf::Vector2i(curr.x, curr.y - 2); //add north
        neighbors[1] = sf::Vector2i(curr.x, curr.y + 2); //add south
        neighbors[2] = sf::Vector2i(curr.x + 2, curr.y); //add east
        neighbors[3] = sf::Vector2i(curr.x - 2, curr.y); //add west

        //shuffle neighbors, swaps each item with a random other 
        for (size_t i = 0; i < 4; i++)
        {
            int s =  rand() % 4;
            sf::Vector2i temp = neighbors[i];
            neighbors[i] = neighbors[s];
            neighbors[s] = temp;
        }

        for(size_t i = 0; i < 4; i++)
        {
            //if neighbor is in bounds and unvisited
            if(!isOOB(neighbors[i].y, neighbors[i].x) && grid[ neighbors[i].y ][ neighbors[i].x ] == '-')
            {
                //return current square to stack
                path.push(curr);
                //open neighbor and seperating wall
                sf::Vector2i wall = betweenSquare(curr, neighbors[i]);
                grid[wall.y][wall.x] = 'o';
                grid[neighbors[i].y][neighbors[i].x] = 'o';
                //push opened wall and neighbor to render queue
                renderQ.push(wall);
                renderQ.push(neighbors[i]);
                draw(renderSpeed);
                //push neighbor to path
                path.push(neighbors[i]);
                break;
            }
        }
    }
    draw(); //empty render queue
}

void PrimMaze::generate()
{
    std::vector<sf::Vector2i> walls;

    int row = 1 + 2 * (rand() % (rowSize / 2));
    int col = 1 + 2 * (rand() % (colSize / 2));
    grid[row][col] = 'o';
    renderQ.push(sf::Vector2i(col,row));
    draw(renderSpeed);
    addWalls(row, col, walls);

    while(!walls.empty())
    {
        draw(renderSpeed);
        int i = rand() % walls.size();
        sf::Vector2i temp = walls[i];
        walls[i] = walls[walls.size()-1];
        walls[walls.size()-1] = temp;
        walls.pop_back();
        addCell(temp.y, temp.x, walls);
    }
}

void PrimMaze::addCell(int row, int col, std::vector<sf::Vector2i> &w)
{

    if(row % 2 == 0)
    {
        if( (grid[row+1][col] != '-') && (grid[row-1][col] == '-') )
        {
            grid[row][col] = 'o';
            renderQ.push(sf::Vector2i(col,row));
            addWalls(row-1, col, w);
        }else if( (grid[row+1][col] == '-') && (grid[row-1][col-1] != '-') )
        {
            grid[row][col] = 'o';
             renderQ.push(sf::Vector2i(col,row));
            addWalls(row+1, col, w);
        }
    }else
    {
        if( (grid[row][col+1] != '-') && (grid[row][col-1] == '-') )
        {
            grid[row][col] = 'o';
            renderQ.push(sf::Vector2i(col,row));
            addWalls(row, col-1, w);
        }else if( (grid[row][col+1] == '-') && (grid[row][col-1] != '-') )
        {
            grid[row][col] = 'o';
            renderQ.push(sf::Vector2i(col,row));
            addWalls(row, col+1, w);
        }
    }
}

void PrimMaze::addWalls(int row, int col, std::vector<sf::Vector2i> &w)
{
    grid[row][col] = 'o';
    renderQ.push(sf::Vector2i(col,row));
    if(row-1 > 1)
    {
        w.push_back(sf::Vector2i(col, row-1));
    }
    if(row+1 < rowSize-1)
    {
        w.push_back(sf::Vector2i(col, row+1));
    }
    if(col+1 < colSize - 1)
    {
        w.push_back(sf::Vector2i(col+1, row));
    }
    if(col-1 > 1)
    {
        w.push_back(sf::Vector2i(col-1, row));
    }
}



