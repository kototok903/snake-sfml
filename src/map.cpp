#include "map.hpp"


std::string Map[MAP_HEIGHT] = {
    "000000000000000000000",
    "0                   0",
    "0   f               0",
    "0                   0",
    "0                   0",
    "0                   0",
    "0                   0",
    "0                   0",
    "0                   0",
    "0                   0",
    "0                   0",
    "0           f       0",
    "0                s  0",
    "0                   0",
    "000000000000000000000",
};

void generateTile(int amount, char tile) {
    srand(static_cast<unsigned>(time(nullptr)));
    int x, y;
    while (amount) {
        x = 1 + rand() % (MAP_WIDTH - 2);
        y = 1 + rand() % (MAP_HEIGHT - 2);
        if (Map[y][x] == ' ') {
            Map[y][x] = tile;
            amount--;
        }
    }
}