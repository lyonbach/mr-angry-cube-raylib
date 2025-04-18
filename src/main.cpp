#include "GameObject.h"
#include "Game.h"
#include "MrAngryCube.h"
#include "Enemy.h"
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
GameConfig config(240, 100, 800, 800, "Mr Angry Cube (Dev Test)",
    texturePath.c_str(), shaderPath.c_str(), modelPath.c_str());

int main(void)
{
    Game* game = new Game(&config);
    int returnCode = game->Run();
    delete game;
    return returnCode;
}
