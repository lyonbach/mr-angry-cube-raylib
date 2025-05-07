#pragma once
#include "Game.h"
#include "raylib.h"
#include "raymath.h"
#include <string>
#include <vector>

class Game;

class Level
{
public:
    std::string name;
    std::vector<GameObject*> staticObjects;  // We store the static object and dynamic object transforms on level load.
    std::vector<GameObject*> dynamicObjects;
    Level(std::string filePath, Game* game);
    ~Level();
    bool loaded = false;
};

