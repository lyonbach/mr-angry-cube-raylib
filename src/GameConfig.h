#pragma once
#include "Miscellaneous.h"
#include "raylib.h"
#include <string>

class GameConfig  // Singleton class
{
public:
    GameConfig(std::string texturePath, std::string shaderPath, std::string modelPath, int screenWidth, int screenHeight, int updateSpeed, Color backgroundColor);
    std::string texturePath;
    std::string shaderPath;
    std::string modelPath;
    int screenWidth;
    int screenHeight;
    int updateSpeed;
    std::string windowTitle = "Mr. AngryCube (DEV)";
    Color backgroundColor;

private:
    static GameConfig s_GameConfig;
};