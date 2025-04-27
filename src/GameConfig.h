#pragma once
#include "raylib.h"
#include <string>
#include <map>

struct GameConfig
{
    std::map<std::string, std::string> texturePaths = {
        {"macDefault", "./assets/textures/test.png"},
        {"enemyDefault", "./assets/textures/concrete.png"},
        {"mainMenuBackground", "./assets/textures/MrCube-GDD.jpg"}
    };

    std::map<std::string, std::string> shaderPaths = {
        {"macDefault", "./assets/shaders/base.vs|./assets/shaders/base.fs"}
    };

    std::map<std::string, std::string> modelPaths = {
        {"macDefault", "./assets/models/mr_angry_cube_high_res.obj"},
        {"enemyDefault", "./assets/models/enemy.obj"}
    };

    Vector2 screenSize = {900, 540};
    Color backgroundColor = BLACK;
    float updateRate = 90.0f;
    float updateTime = 1.0f / updateRate;
    std::string windowTitle = "Mr. Angry Cube";
};
