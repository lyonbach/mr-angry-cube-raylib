#include "GameConfig.h"

GameConfig::GameConfig(std::string texturePath, std::string shaderPath, std::string modelPath, int screenWidth, int screenHeight, int updateSpeed, Color backgroundColor)
    : texturePath(texturePath),
      shaderPath(shaderPath),
      modelPath(modelPath),
      screenWidth(screenWidth),
      screenHeight(screenHeight),
      updateSpeed(updateSpeed),
      backgroundColor(backgroundColor)
{
    Utilities::Log("Created game configuration.", "GAMECONFIG");
}
