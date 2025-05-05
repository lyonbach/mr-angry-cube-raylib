#pragma once
#include "raylib.h"
#include "raymath.h"
#include <string>
#include <vector>


class Level
{
public:
    std::string filePath;
    std::vector<Matrix> staticObjects;  // We store the static object and dynamic object transforms on level load.
    std::vector<Matrix> dynamicObjects;
    Level(std::string filePath);
    ~Level();
};