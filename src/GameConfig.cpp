#include "GameConfig.h"

GameConfig::GameConfig()
{

}

GameConfig& GameConfig::Get()
{
    return s_GameConfig;
}

void GameConfig::Init(std::string texturePath, std::string shaderPath, std::string modelPath, int screenWidth, int screenHeight, int updateSpeed, Color backgroundColor)
{
    this->texturePath = texturePath;
    this->shaderPath = shaderPath;
    this->modelPath = modelPath;
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->updateSpeed = updateSpeed;
    this->backgroundColor = backgroundColor;
    initialized = true;
}

