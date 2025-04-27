#include "Behaviours.h"
#include "Enemy.h"
#include "Game.h"
#include "GameConfig.h"
#include "GameObject.h"
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

int main(void)
{
    const char* wd = GetWorkingDirectory();

    int updateRate = 90;  
    #ifdef PLATFORM_WEB
        updateRate = 120;
        TraceLog(LOG_INFO, "Setting everything for webplatform...");
    #endif

    Game& game = Game::Get();

    // GameConfig gameConfig(960*2, 540*2, updateRate, DARKGRAY);
    GameConfig gameConfig(960, 540, updateRate, DARKGRAY);
    gameConfig.modelPaths["macDefault"] = "./assets/models/mr_angry_cube_high_res.obj";
    gameConfig.modelPaths["enemyDefault"] = "./assets/models/enemy.obj";
    gameConfig.texturePaths["macDefault"] = "./assets/textures/texel_checker.png";
    gameConfig.texturePaths["enemyDefault"] = "./assets/textures/concrete.png";
    gameConfig.texturePaths["mainMenuBackground"] = "./assets/textures/MrCube-GDD.jpg";
    gameConfig.shaderPaths["macDefaultFS"] = "./assets/shaders/base.fs";
    gameConfig.shaderPaths["macDefaultVS"] = "./assets/shaders/base.vs";

    gameConfig.modelPaths["column0"] = "./assets/models/testColumn.obj";
    game.Init(&gameConfig);

    int returnCode = game.Run();
    return returnCode;
}
