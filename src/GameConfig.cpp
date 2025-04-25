#include "GameConfig.h"

GameConfig::GameConfig(
    int screenWidth,
    int screenHeight,
    int updateSpeed,
    Color backgroundColor,
    std::string windowTitle
)
    :
    screenWidth(screenWidth),
    screenHeight(screenHeight),
    updateSpeed(updateSpeed),
    backgroundColor(backgroundColor),
    windowTitle(windowTitle)
{
    Utilities::Log("Created game configuration.", "GAMECONFIG");
}
