#pragma once
#define RINI_IMPLEMENTATION
#include "raylib.h"
#include <string>
#include <map>


struct GameConfig
{
    GameConfig();
    GameConfig(const char* configPath);
    void LogConfig();
    void Init();

    std::map<std::string, std::string> texturePaths = {  // TODO MOVE TO game.ini
        {"macDefault", "./assets/textures/concrete.png"},
        {"enemyDefault", "./assets/textures/texel_checker_crayon.png"},
        {"mainMenuBackground", "./assets/textures/mr-angry-cube-artwork.png"},
        {"levelSelectionMenuBackground", "./assets/textures/mr-angry-cube-artwork-level-select.png"}  // TODO REAL GAME ASSET.
    };

    std::map<std::string, std::string> shaderPaths = {  // TODO MOVE TO game.ini
        {"macDefault", "./assets/shaders/base.vs|./assets/shaders/base.fs"},
        {"staticObjectDefault", "./assets/shaders/base.vs|./assets/shaders/base.fs"},
    };

    std::map<std::string, std::string> modelPaths = {  // TODO MOVE TO game.ini
        {"macDefault", "./assets/models/mr_angry_cube.obj"},
        {"enemyDefault", "./assets/models/enemy.obj"},
        {"column", "./assets/models/column.obj"}
    };

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
