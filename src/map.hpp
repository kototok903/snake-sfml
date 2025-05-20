#pragma once

#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

const int MAP_HEIGHT = 15;
const int MAP_WIDTH = 21;

extern std::string Map[MAP_HEIGHT];

void generateTile(int amount, char tile);
