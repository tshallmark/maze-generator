#include <SFML/Graphics.hpp>
#include "mazeClient.h"

MazeClient::MazeClient(sf::RenderWindow* w)
{
    if (!font.loadFromFile("source/consola.ttf"))
    {
        std::cout << "missing ttf\n";
    }

    window = w;
}

int MazeClient::menu(const std::string options[], const int numOptions)
{
    sf::Event event;
    sf::Text text;
    text.setFont(font);

    sf::RectangleShape block;
    sf::Vector2f blockSize;
    blockSize.x = window->getSize().x ;
    blockSize.y = window->getSize().y / numOptions;

    block.setFillColor(LOW);
    text.setFillColor(HIGH);
    block.setSize(blockSize);
    block.setOutlineThickness(-3);

    int posY;

    int selection = 0;
    
    while(1)
    {
        while(window->pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window->close();
                exit(0);
            }
            if(event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up)
                {
                    selection = (selection + numOptions-1) % numOptions;
                }
                if (event.key.code == sf::Keyboard::Down)
                {
                    selection = (selection + 1) % numOptions;
                }
                if (event.key.code == sf::Keyboard::Enter)
                {
                    window->clear(sf::Color::Black);
                    return selection;
                }
            }
        }

        for (size_t i = 0; i < numOptions; i++)
        {
            
            posY = 0 + ((float)i * blockSize.y);
            text.setString(options[i]);
            block.setPosition(0, posY);
            text.setPosition(10, posY + (blockSize.y/2) );
            if(i == selection)
            {
                block.setFillColor(HIGH);
                text.setFillColor(LOW);
                block.setOutlineColor(sf::Color::Red);
                block.setOutlineThickness(-3);
            }else
            {
                block.setFillColor(LOW);
                text.setFillColor(HIGH);
                block.setOutlineColor(sf::Color::Black);
                block.setOutlineThickness(-3);
            }
            
            window->draw(block);
            window->draw(text);
            window->display();
        }
    }
    return 0;
}