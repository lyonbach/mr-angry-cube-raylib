#pragma once
#include "Miscellaneous.h"
#include "raylib.h"
#include <string>

class GameConfig  // Singleton class
{
public:
    std::string texturePath;
    std::string shaderPath;
    std::string modelPath;
    int screenWidth;
    int screenHeight;
    int updateSpeed;
    std::string windowTitle;
    Color backgroundColor;
    GameConfig(
        std::string texturePath,
        std::string shaderPath,
        std::string modelPath,
        int screenWidth,
        int screenHeight,
        int updateSpeed,
        Color backgroundColor,
        std::string windowTitle="Mr. AngryCube (DEV)"
    );
};