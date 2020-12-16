#ifndef MAZECLIENT_H
#define MAZECLIENT_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

const sf::Color HIGH(200,200,200);
const sf::Color LOW(50,50,50);


class MazeClient
{
public:
    MazeClient(sf::RenderWindow* w);

    int menu(const std::string options[], const int count);
private:
    sf::RenderWindow* window; //pointer to sfml window

    sf::Font font;
};

#endif