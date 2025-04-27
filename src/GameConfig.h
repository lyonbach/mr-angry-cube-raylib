#pragma once
#include "raylib.h"
#include <string>
#include <map>

class GameConfig
{
public:
    std::map<std::string, std::string> texturePaths;
    std::map<std::string, std::string> shaderPaths;
    std::map<std::string, std::string> modelPaths;
    Vector2 screenSize;
    Color backgroundColor;
    int updateRate;
    float updateTime;
    const char* windowTitle;

    GameConfig(Vector2 screenSize, Color backgroundColor);
};
