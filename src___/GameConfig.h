#pragma once
#include "Miscellaneous.h"
#include "raylib.h"
#include <string>
#include <map>

class GameConfig
{
public:
    std::map<std::string, std::string> texturePaths;
    std::map<std::string, std::string> shaderPaths;
    std::map<std::string, std::string> modelPaths;
    int screenWidth;
    int screenHeight;
    int updateSpeed;
    std::string windowTitle;
    Color backgroundColor;

    GameConfig(
        int screenWidth,
        int screenHeight,
        int updateSpeed,
        Color backgroundColor,
        std::string windowTitle="Mr. AngryCube (DEV)"
    );
};
