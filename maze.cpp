/**
 * @file maze.h
 * @author Taylor Hallmark
 * @brief definition file for maze class
 * @date 12/10
 */

#include "maze.h"

/**
 * @brief Maze class cconstructor
 * 
 * @param w pointer to an sfml window
 * @param cols width of the maze in squares
 * @param rows height of the maze in squares
 * 
 * @return 
 */
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

    std::cout << window->getSize().x << ' ';
    std::cout << window->getSize().y << '\n';

    std::cout << colSize << ' ';
    std::cout << rowSize << '\n';
    
    std::cout << squareSize.x << ' ';
    std::cout << squareSize.y << '\n';

    //allocate memory for main array
    grid = new char*[rowSize];
    for(int row = 0; row < rowSize; row++)
    {
        grid[row] = new char[colSize];
        for (size_t col = 0; col < colSize; col++)
        {
            initializeSquare(row,col);
            //push each square to render queue
            renderQ.push(sf::Vector2i(col,row));
        }
    }

    //place start & end on random square of top & bottom row
    start = 1 + 2 * (rand() % (colSize / 2));
    end = 1 + 2 * (rand() % (colSize / 2));

    grid[0][start] = 's';
    grid[rowSize-1][end] = 'e';
}

/**
 * @brief Maze class destructor
 */
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

/**
 * @brief sets the number of updates required before rendering during
 * & solving
 * 
 * @param s new renderSpeed
 */
void Maze::setRenderSpeed(int s)
{
    //validate speed
    if(s >= 0)
        renderSpeed = s;
}

/**
 * @brief initializes value of squares in grid[][] before generation
 * 
 * @param row row of the target
 * @param col column of the targer
 */
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

/**
 * @brief finds path through maze by depth-first search
 * 
 */
void Maze::depthPath()
{
    int maxDepth = 0;
    std::stack<sf::Vector2i> path;
    sf::Vector2i curr(start, 1);
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
            return;

        grid[curr.y][curr.x] = 'p';
        sf::Vector2i north(curr.x, curr.y - 1);
        sf::Vector2i south(curr.x, curr.y + 1);
        sf::Vector2i east(curr.x + 1, curr.y);
        sf::Vector2i west(curr.x - 1, curr.y);

        if(validMove(north) ) //check north
        {
            path.push(curr);
            path.push(north);
            
        }else if( validMove(south) ) //check south
        {
            path.push(curr);
            path.push(south);
        }else if( validMove(east) ) //check east
        {
            path.push(curr);
            path.push(east);
        }else if( validMove(west) ) //check west
        {
            path.push(curr);
            path.push(west);
        }else
        {
            grid[curr.y][curr.x] = 'f';
        }
        renderQ.push(curr);
        draw(renderSpeed);
    }
}

void Maze::breadthPath()
{
    std::queue<pathNode*> toVisit;
    std::list<pathNode> graph;

    sf::Vector2i temp(start, 1);
    graph.push_back(pathNode(temp, nullptr));
    toVisit.push(&graph.back());

    while(!toVisit.empty())
    {
        pathNode* nodePtr = toVisit.front();
        toVisit.pop();
        sf::Vector2i curr = nodePtr->positon;


        if(grid[curr.y][curr.x] == 'e')
        {
            drawPath(nodePtr);
            break;
        }else
        {
            grid[curr.y][curr.x] = 'f';
            renderQ.push(curr);
            draw(renderSpeed);
        }
        

        sf::Vector2i north(curr.x, curr.y - 1);
        sf::Vector2i south(curr.x, curr.y + 1);
        sf::Vector2i east(curr.x + 1, curr.y);
        sf::Vector2i west(curr.x - 1, curr.y);

        if(validMove(north) ) //check north
        {
            graph.push_back(pathNode(north, nodePtr));
            toVisit.push(&graph.back());
        }
        if( validMove(south) ) //check south
        {
            graph.push_back(pathNode(south, nodePtr));
            toVisit.push(&graph.back());
        }
        if( validMove(east) ) //check east
        {
            graph.push_back(pathNode(east, nodePtr));
            toVisit.push(&graph.back());
        }
        if( validMove(west) ) //check west
        {
            graph.push_back(pathNode(west, nodePtr));
            toVisit.push(&graph.back());
        }
    }
}

void Maze::drawPath(pathNode* nodePtr)
{
    printGrid(std::cout);
    nodePtr = nodePtr->parent;
    while(nodePtr != nullptr)
    {
        sf::Vector2i temp = nodePtr->positon;
        grid[temp.y][temp.x] = 'p';
        renderQ.push(temp);
        nodePtr = nodePtr->parent;
        draw(renderSpeed);
    }
}

/**
 * @brief returns true if given coordinates are a valid path
 * 
 * @param row target row
 * @param col taret column
 * 
 * @return true if square is part of a potential path
 */
bool Maze::validMove(sf::Vector2i loc)
{
    int row = loc.y;
    int col = loc.x;
    //return true if
    return ( !isOOB(row, col) //not out of bounds
        && grid[row][col] != '#' // and not a wall 
        && grid[row][col] != 'p' // and not on current path
        && grid[row][col] != 'f' // and not on failed path
        && grid[row][col] != 's'); 
}

/**
 * @brief checks if a square is out of bounds on grid[]][]
 * 
 * @param row target row
 * @param col target column
 * 
 * @return true if out of bounds
 */
bool Maze::isOOB(int row, int col)
{
    if(col < 0 || col >= colSize || row < 0 || row >= rowSize)
        return true;
    return false;
}

/**
 * @brief locates the square between two adjacent coordinates
 * used to open walls during generation
 * 
 * @param a vector of the current square
 * @param b vector of the target square
 * 
 * @return vector of seperating wall
 */
sf::Vector2i Maze::betweenSquare(sf::Vector2i a,sf::Vector2i b)
{
    //set between coordinates to half the difference of a & b coordinates
    sf::Vector2i between( (b.x - a.x)/2, (b.y - a.y)/2 );

    //add between vector back to a
    between.x += a.x;
    between.y += a.y;

    return between;
}

/**
 * @brief fully clears and refreshes the window
 * 
 */
void Maze::refresh()
{
    window->clear(sf::Color::Black);
    updateAll();
    draw();
}


/**
 * @brief removes paths from maze
 * 
 */
void Maze::reset()
{
    for(size_t row = 0; row < rowSize; row++)
    {
        for (size_t col = 0; col < colSize; col++)
        {
            if(grid[row][col] == 'p' || grid[row][col] == 'f')
            {
                grid[row][col] = 'o';
                renderQ.push(sf::Vector2i(col, row));
            }
        }
    }
}

/**
 * @brief adds all squares to the render queue
 * 
 */
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

/**
 * @brief Draws the current contents of the renderqueue if queue is larger than threshold
 * except in the case of a window resize, in which the window is fully redrawn or window close
 * 
 * @param threshold minimum updated squares for normal rendering
 */
void Maze::draw(int threshold)
{
    sf::Event event;
    while(window->pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            window->close();
            exit(0);
        }

        if (event.type == sf::Event::Resized)
        {
            //recalculate squaresize
            squareSize.x = event.size.width / (float)colSize;
            squareSize.y = event.size.height / (float)rowSize;
            updateAll();
            threshold = 0;
            window->clear(sf::Color::Black);
        }
    }

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
            float posX = 0 + ((float)temp.x * squareSize.x);
            float posY = 0 + ((float)temp.y * squareSize.y);
            
            drawSquare(grid[temp.y][temp.x], posX, posY);
        }
        window->display();
    }
}

/**
 * @brief draws a single square into window
 * 
 * @param c contents of the square in grid[][]
 * @param posX x position of the square in the window
 * @param posY y position of the square in the window
 * 
 * @return 
 */
void Maze::drawSquare(char c, float posX, float posY)
{
    //create and place shape
    sf::RectangleShape square(squareSize);
    square.setPosition(posX, posY);

    if(c == '#' || c == '-') //color walls black
        square.setFillColor(sf::Color::Black);
    else if (c == 'o') //open squares white
    {
        square.setFillColor(grey);
    }else if (c == 'f') //failed paths / potenial paths
    {
        square.setFillColor(sf::Color::Cyan);
    }else if (c == 'p') //active paths
    {
        square.setFillColor(sf::Color::Blue);
    }else if (c == 's') //start
    {
        square.setFillColor(sf::Color::Green);
    }else if (c =='e') //end
    {
        square.setFillColor(sf::Color::Red);
    }
    
    window ->draw(square);
}

/**
 * @brief prints the raw char grid into ostream, useful for debuging
 * 
 * @param ostr destination stream
 * 
 */
void Maze::printGrid(std::ostream &ostr)
{
    std::cout << "  `";
    for (size_t i = 0; i < colSize ; i++)
    {
        std::cout << i << ' ';
    }
    std::cout << '\n';
    
    for (size_t row = 0; row < rowSize; row++)
    {
        std::cout << row << ' ';
        for (size_t col = 0; col < colSize; col++)
        {
            ostr << grid[row][col] << ' ';
        }
        ostr << "\n";
    }
    ostr << "\n";
}

/**
 * @brief generates maze by depth full depth first search of grid[][]
 * 
 */
void DepthMaze::generate()
{
    int maxDepth = 0; //maximum search depth 

    //find random starting location
    int row = 1 + 2 * (rand() % (rowSize / 2));
    int col = 1 + 2 * (rand() % (colSize / 2));

    std::stack<sf::Vector2i> path; //stack of squares in active path

    sf::Vector2i curr(col, row); //current square
    grid[row][col] = 'o';
    renderQ.push(curr); //add current square to render queue
    path.push(curr); //add current square to path

    //continue search until path is empty
    while(!path.empty()) 
    {
        //update maxDepth
        if(path.size() > maxDepth)
            maxDepth = path.size();
        
        //get and pop new current square from stack
        curr = path.top();
        path.pop();
        sf::Vector2i neighbors[4]; //array of coordinates of adjacent squares
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

/**
 * @brief generate maze by a randomized prim's algorithm
 */
void PrimMaze::generate()
{
    std::vector<sf::Vector2i> walls; //list of walls 

    //select random starting square
    int row = 1 + 2 * (rand() % (rowSize / 2));
    int col = 1 + 2 * (rand() % (colSize / 2));
    grid[row][col] = 'o'; //mark as open
    renderQ.push(sf::Vector2i(col,row)); 
    draw(renderSpeed);
    //add walls around current square to list
    addWalls(row, col, walls);

    while(!walls.empty())
    {
        draw(renderSpeed);
        //select random wall from list
        int i = rand() % walls.size();
        //remove wall from list
        sf::Vector2i temp = walls[i];
        walls[i] = walls[walls.size()-1];
        walls[walls.size()-1] = temp;
        walls.pop_back();
        //add cells adjacent to current wall
        addCell(temp.y, temp.x, walls);
    }
}

/**
 * @brief //adds unvisited cell adjacent to wall if exists
 * 
 * @param row row of target wall
 * @param col col of target wall
 * @param w reference to wall list 
 * 
 * @return 
 */
void PrimMaze::addCell(int row, int col, std::vector<sf::Vector2i> &w)
{

    if(row % 2 == 0) //if wall seperates squares vertically
    {
        //if only one adjacent square is unvisited, mark it as open and
        //add adjacent walls to list
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
    }else //if wall seperates squares horizontally
    {
        //if only one adjacent square is unvisited, mark it as open and
        //add adjacent walls to list
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

/**
 * @brief adds walls adjacent to a cell into list
 * 
 * @param row row of target cell
 * @param col column of target cell
 * @param w @param w reference to wall list 
 */
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

void TestMaze::generate()
{
    start = 5;
    char test[7][7] = {
        {'#','#','#','#','#','s','#'},
        {'#','o','o','o','o','o','#'},
        {'#','o','#','o','#','#','#'},
        {'#','o','#','o','o','o','#'},
        {'#','o','#','#','#','#','#'},
        {'#','o','o','o','o','o','#'},
        {'#','e','#','#','#','#','#'}
    };

    for (size_t row = 0; row < 7; row++)
    {
        for (size_t col = 0; col < 7; col++)
        {
            grid[row][col] = test[row][col];
        }
        
    }

};

void TestMaze::printValidityGrid(std::ostream &ostr)
{
    std::cout << "  ";
    for (size_t i = 0; i < colSize ; i++)
    {
        std::cout << i << ' ';
    }
    std::cout << '\n';
    
    for (size_t row = 0; row < rowSize; row++)
    {
        std::cout << row << ' ';
        for (size_t col = 0; col < colSize; col++)
        {
            sf::Vector2i temp(col, row);
            ostr << !validMove(temp) << ' ';
        }
        ostr << "\n";
    }
    ostr << "\n";
}

void TestMaze::testPath(std::vector<sf::Vector2i> path)
{
    std::list<pathNode> pList;
    pList.push_back(pathNode(path[0], nullptr));
    for (size_t i = 1; i < path.size(); i++)
    {
        pList.push_back(pathNode( path[i], &pList.back() ));
    }
    drawPath(&pList.back());
}