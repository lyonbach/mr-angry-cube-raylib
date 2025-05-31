#pragma once
#define RINI_IMPLEMENTATION
#include "Constants.h"
#include "Utilities.h"
#include "raylib.h"
#include <string>
#include <map>
#include <filesystem>

namespace fs = std::filesystem;

struct GameConfig
{
    GameConfig();
    GameConfig(const char* configPath);
    void LogConfig();
    void LogFilePaths(std::map<std::string, std::string>& paths, std::string typeName) const;
    void Init();

    std::map<std::string, std::string> texturePaths;
    std::map<std::string, std::string> shaderPaths;
    std::map<std::string, std::string> vertexShaderPaths;
    std::map<std::string, std::string> fragmentShaderPaths;
    std::map<std::string, std::string> modelPaths;

    const char* configPath;
    Vector2 screenSize = {640, 480};
    bool fullScreen = 0;
    float updateRate = 90.0f;
    float warmUpTime = 2.0f;
    float updateTime;

    bool configured = false;
    Color backgroundColor = DARKGRAY;
    std::string windowTitle = "Mr. Angry Cube";

};
