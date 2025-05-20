#include "snake.hpp"


Snake::Snake(const std::string& fileName, float X, float Y, float W, float H) 
    : textureFileName(fileName),
    w(W),
    h(H),
    coffeeTime(0),
    dir(STOP),
    eat(NOTHING),
    fruitScore(0),
    health(60),
    life(true),
    texture(),
    sprite([&] {
        sf::Image image;
        if (!image.loadFromFile(fileName)) {
            std::cerr << "Failed to load snake image\n";
        }
        texture.loadFromImage(image);
        sf::Sprite s(texture);
        s.setTextureRect(sf::IntRect({0, 0}, {static_cast<int>(W), static_cast<int>(H)}));
        return s;
    }())
{
    snake.push_back({X, Y});
}


void Snake::update() {
    if (!life) return;

    sf::Vector2f newHead = snake[0];
    switch (dir) {
        case LEFT:  newHead.x -= speed * 32; break;
        case RIGHT: newHead.x += speed * 32; break;
        case UP:    newHead.y -= speed * 32; break;
        case DOWN:  newHead.y += speed * 32; break;
        default: break;
    }

    srand(static_cast<unsigned>(time(nullptr)));
    int x = static_cast<int>(newHead.x) / 32;
    int y = static_cast<int>(newHead.y) / 32;
    int r;

    if (Map[y][x] == 'f') {
        eat = FRUIT;
        fruitScore++;
        generateTile(1, 'f');
        r = rand() % 10;
        if (!r) generateTile(1, 'c');
        Map[y][x] = ' ';
    }

    if (Map[y][x] == 'c') {
        coffeeTime = 8000;
        Map[y][x] = ' ';
    }

    if (Map[y][x] == 's') {
        dir = STOP;
        Map[y][x] = ' ';
    }

    if (dir != Direction::STOP) {
        for (int i = 0; i < snake.size(); i++) {
            if (snake[i].x == newHead.x && snake[i].y == newHead.y) {
                life = false;
                break;
            }
        }
    }

    if (Map[y][x] == ' ') {
        move(newHead);
    } else if (Map[y][x] == '0') {
        life = false;
    } 

    if (!life) speed = 0;
}


void Snake::move(const sf::Vector2f &newHead) {
    snake.insert(snake.begin(), newHead);
    if (eat != Food::FRUIT) snake.pop_back();
    eat = NOTHING;
}