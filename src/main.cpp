#include "Game.h"
#include "GameConfig.h"
#include <memory>


int main()
{
    const char* wd = GetWorkingDirectory();

    // Create game configuration.
    GameConfig gameConfig;

    Game* gameInstance = &Game::Get();
    gameInstance->Init(gameConfig);
    return gameInstance->Run();
}