#include "Enemy.h"
#include "Game.h"
#include "GameConfig.h"
#include "GameObject.h"
#include "Behaviours.h"
#include "MrAngryCube.h"
#include "SimpleGui.h"
#include "raylib.h"
#include "raymath.h"
#include <vector>

// TODO
// #ifdef PLATFORM_WEB
//     #define GLSL_VERSION 330
// #else
//     #define GLSL_VERSION 110
// #endif


std::string texturePath = "./assets/textures/texel_checker.png";
std::string shaderPath  = "./assets/shaders/base.fs";
std::string modelPath   = "./assets/models/mr_angry_cube_high_res.obj";

int main(void)
{
    const char* wd = GetWorkingDirectory();

    int updateRate = 90;
    #ifdef PLATFORM_WEB
        updateRate = 90;
        TraceLog(LOG_INFO, "Setting everything for webplatform...");
    #endif

    Game& game = Game::Get();
    GameConfig gameConfig(texturePath, shaderPath, modelPath, 1024, 768, updateRate, DARKGRAY);
    game.Init(&gameConfig);

    int returnCode = game.Run();
    return returnCode;
}
