#pragma once
#include "raylib.h"
#include <string>

class GameConfig  // Singleton class
{
public:
    static GameConfig& Get();
    std::string texturePath;
    std::string shaderPath;
    std::string modelPath;
    int screenWidth;
    int screenHeight;
    int updateSpeed;
    std::string windowTitle = "Mr. AngryCube (DEV)";
    Color backgroundColor;
    void Init(std::string texturePath, std::string shaderPath, std::string modelPath, int screenWidth, int screenHeight, int updateSpeed, Color backgroundColor);
    bool initialized = false;
private:
    static GameConfig s_GameConfig;
    GameConfig();
};