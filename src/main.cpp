#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>
#include <ctime>
#include <string>
#include "map.h"

const unsigned int WINDOW_WIDTH = 672, WINDOW_HEIGHT = 480;
const float SNAKE_WIDTH = 32.0f, SNAKE_HEIGHT = 32.0f;

// struct Tile {
//     float x, y;
// };

enum Direction { STOP, LEFT, RIGHT, UP, DOWN };
enum Food { NOTHING, FRUIT };

class Snake {
public:
    sf::Vector2f head;
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

    Snake(const std::string& fileName, float X, float Y, float W, float H) 
      : textureFileName(fileName),
        head{X, Y},
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
            if (!image.loadFromFile("images/" + fileName)) {
                std::cerr << "Failed to load snake image\n";
            }
            texture.loadFromImage(image);
            sf::Sprite s(texture);
            s.setTextureRect(sf::IntRect({0, 0}, {static_cast<int>(W), static_cast<int>(H)}));
            return s;
        }())
    {
        snake.push_back(head);
    }


    void update() {
        switch (dir) {
            case LEFT:  head.x -= speed * 32; break;
            case RIGHT: head.x += speed * 32; break;
            case UP:    head.y -= speed * 32; break;
            case DOWN:  head.y += speed * 32; break;
            default: break;
        }

        interactionWithMap();

        if (!life) speed = 0;
    }

    void interactionWithMap() {
        srand(static_cast<unsigned>(time(nullptr)));
        int x = static_cast<int>(head.x) / 32;
        int y = static_cast<int>(head.y) / 32;
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

        if (Map[y][x] == ' ') {
            move();
        } else if (Map[y][x] == '0') {
            life = false;
        } else {
            for (int i = 0; i < snake.size(); i++) {
                if (snake[i].x == head.x && snake[i].y == head.y) {
                    life = false;
                    break;
                }
            }
        }
    }

    void move() {
        switch (eat) {
            case NOTHING:
                snake.insert(snake.begin(), head);
                snake.pop_back();
                break;
            case FRUIT:
                snake.insert(snake.begin(), head);
                break;
        }
        eat = NOTHING;
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Snake");

    // icon
    sf::Image icon;
    if (icon.loadFromFile("images/snakeicon.png"))
        window.setIcon(icon.getSize(), icon.getPixelsPtr());

    // player
    Snake S("snake0.1.png", 10 * 32.0f, 7 * 32.0f, SNAKE_WIDTH, SNAKE_HEIGHT);

    // map
    sf::Image mapImage;
    sf::Texture mapTexture;
    if (!mapImage.loadFromFile("images/snakemapv2.png")) {
        std::cerr << "Failed to load map image\n";
        return -1;
    }
    mapTexture.loadFromImage(mapImage);
    sf::Sprite map(mapTexture);

    // fonts and text
    sf::Font fontOld, fontGemu, fontArcade;
    fontOld.openFromFile("fonts/CyrilicOld.ttf");
    fontGemu.openFromFile("fonts/GemunuLibre-Regular.ttf");
    fontArcade.openFromFile("fonts/ArcadeClassic.ttf");

    sf::Text text(fontArcade, "", 28);
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Bold);
    text.setOutlineColor(sf::Color::White);
    text.setOutlineThickness(2);

    sf::Text redtext(fontOld, "", 18);
    redtext.setFillColor(sf::Color::Red);
    redtext.setStyle(sf::Text::Bold);
    redtext.setOutlineColor(sf::Color::White);
    redtext.setOutlineThickness(2);

    sf::Text deathtext(fontOld, "", 80);
    deathtext.setFillColor(sf::Color::Red);
    deathtext.setStyle(sf::Text::Bold);

    // Tab
    sf::Image tabImage;
    sf::Texture tabTexture;
    bool isTab = false;
    if (!tabImage.loadFromFile("images/Tab.png")) {
        std::cerr << "Failed to load tab image\n";
        return -1;
    }
    tabTexture.loadFromImage(tabImage);
    sf::Sprite Tab(tabTexture);

    sf::Clock clock;
    sf::Clock gameTimeClock;
    float time;
    int gameTime = 0;

    while (window.isOpen()) {
        if (S.life)
            gameTime = static_cast<int>(gameTimeClock.getElapsedTime().asSeconds());

        time = clock.restart().asMilliseconds();

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (S.life) {
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Left && S.dir != RIGHT) {
                        S.dir = LEFT;
                        S.speed = 1;
                    } else if (keyPressed->scancode == sf::Keyboard::Scancode::Right && S.dir != LEFT) {
                        S.dir = RIGHT;
                        S.speed = 1;
                    } else if (keyPressed->scancode == sf::Keyboard::Scancode::Up && S.dir != DOWN) {
                        S.dir = UP;
                        S.speed = 1;
                    } else if (keyPressed->scancode == sf::Keyboard::Scancode::Down && S.dir != UP) {
                        S.dir = DOWN;
                        S.speed = 1;
                    }
                }
                    
                if (keyPressed->scancode == sf::Keyboard::Scancode::Tab)
                    isTab = !isTab;
            }
        }

        window.clear(sf::Color(128, 106, 89));

        // draw map
        for (int i = 0; i < MAP_HEIGHT; i++) {
            for (int j = 0; j < MAP_WIDTH; j++) {
                if (Map[i][j] == ' ') map.setTextureRect(sf::IntRect({0, 0}, {32, 32}));
                if (Map[i][j] == 'f') map.setTextureRect(sf::IntRect({64, 0}, {32, 32}));
                if (Map[i][j] == 'c') map.setTextureRect(sf::IntRect({96, 0}, {32, 32}));
                if (Map[i][j] == 's') map.setTextureRect(sf::IntRect({128, 0}, {32, 32}));
                if (Map[i][j] == '0') map.setTextureRect(sf::IntRect({32, 0}, {32, 32}));

                map.setPosition({j * 32.0f, i * 32.0f});
                window.draw(map);
            }
        }

        // Snake update and draw
        S.update();
        if (S.life)
            S.sprite.setTextureRect(sf::IntRect({0, 0}, {32, 32}));
        else
            S.sprite.setTextureRect(sf::IntRect({96, 0}, {32, 32}));
        S.sprite.setPosition(S.snake[0]);
        window.draw(S.sprite);

        // body
        if (S.snake.size() > 2) {
            S.sprite.setTextureRect(sf::IntRect({32, 0}, {32, 32}));
            for (int i = 1; i < S.snake.size() - 1; i++) {
                S.sprite.setPosition(S.snake[i]);
                window.draw(S.sprite);
            }
        }

        // tail
        if (S.snake.size() > 1) {
            S.sprite.setTextureRect(sf::IntRect({64, 0}, {32, 32}));
            S.sprite.setPosition(S.snake[S.snake.size() - 1]);
            window.draw(S.sprite);
        }
        
        std::cout << "head: " << S.head.x << " " << S.head.y << "\n"; 
        std::cout << "snake: "; 
        for (int i = 0; i < S.snake.size(); i++) {
            std::cout << S.snake[i].x << " " << S.snake[i].y << ", "; 
        }
        std::cout << "\n";

        // score text
        std::stringstream ss;
        ss << S.fruitScore;
        text.setString("Score " + ss.str());
        text.setPosition({16, 8});
        window.draw(text);

        // death message
        if (!S.life) {
            deathtext.setString("You Dead");
            deathtext.setPosition({WINDOW_WIDTH / 4, WINDOW_HEIGHT / 3});
            window.draw(deathtext);
        }

        // tab overlay
        if (isTab) {
            Tab.setPosition({WINDOW_WIDTH / 3, WINDOW_HEIGHT / 4});
            window.draw(Tab);
        }

        window.display();

        if (S.coffeeTime > 0) {
            sf::sleep(sf::milliseconds(100));
            S.coffeeTime -= time;
        } else {
            sf::sleep(sf::milliseconds(200));
        }
    }

    return 0;
}
