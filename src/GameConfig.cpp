#include "GameConfig.h"
#include"raylib.h"


GameConfig::GameConfig(Vector2 screenSize, Color backgroundColor)
: screenSize(screenSize),
  backgroundColor(backgroundColor),
  windowTitle("Mr. Angry Cube")
{
modelPaths["macDefault"] = "./assets/models/mr_angry_cube_high_res.obj";
modelPaths["enemyDefault"] = "./assets/models/enemy.obj";

texturePaths["macDefault"] = "./assets/textures/texel_checker.png";
texturePaths["enemyDefault"] = "./assets/textures/concrete.png";
texturePaths["mainMenuBackground"] = "./assets/textures/MrCube-GDD.jpg";

shaderPaths["macDefault"] = "./assets/shaders/base.vs|./assets/shaders/base.fs";
// gameConfig.modelPaths["column0"] = "./assets/models/testColumn.obj";
updateTime = 1.0f / updateRate;
}