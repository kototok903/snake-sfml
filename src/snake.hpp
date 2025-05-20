#pragma once 

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>
#include <ctime>
#include <string>
#include "map.hpp"

enum Direction { STOP, LEFT, RIGHT, UP, DOWN };
enum Food { NOTHING, FRUIT };


class Snake {
public:
std::vector<sf::Vector2f> snake;
    int x, y;
    float w, h;
    float coffeeTime;
    Direction dir;
    Food eat;
    int speed, fruitScore, health;
    bool life;
    std::string textureFileName;
    sf::Texture texture;
    sf::Sprite sprite;

    Snake(const std::string& fileName, float X, float Y, float W, float H);
    void update();
    void move(const sf::Vector2f &newHead);
    //void draw(sf::RenderWindow& window);
};
