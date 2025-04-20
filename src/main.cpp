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
std::string texturePath = (fs / "../textures" / "metal.png").string();
std::string shaderPath  = (fs / "../vendor/raylib/examples/shaders/resources/shaders/glsl330" / "blur.fs").string();
std::string modelPath   = (fs / "../models" / "mr_angry_cube_high_res.obj").string();


int main(void)
{
    Game& game = Game::Get();
    GameConfig configuration(texturePath, shaderPath, modelPath, 800, 600, 60, DARKGRAY);
    game.Init(&configuration);

    int returnCode = game.Run();
    return returnCode;
}
