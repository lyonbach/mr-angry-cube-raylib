#include "Enemy.h"
#include "Game.h"
#include "GameConfig.h"
#include "GameObject.h"
#include "MrAngryCube.h"
#include "SimpleGui.h"
#include "raylib.h"
#include "raymath.h"
#include <filesystem>
#include <vector>
#define GLSL_VERSION 330


std::filesystem::path fs = std::filesystem::path(__FILE__).parent_path();
std::string texturePath = (fs / "../textures" / "texel_checker.png").string();
std::string shaderPath  = (fs / "../vendor/raylib/examples/shaders/resources/shaders/glsl330" / "blur.fs").string();
std::string modelPath   = (fs / "../models" / "mr_angry_cube_high_res.obj").string();
GameConfig GameConfig::s_GameConfig;

int main(void)
{
    GameConfig& config = GameConfig::Get();
    config.Init(texturePath, shaderPath, modelPath, 800, 600, 120, DARKGRAY);

    Game* game = new Game();
    int returnCode = game->Run();
    delete game;
    return returnCode;
}
