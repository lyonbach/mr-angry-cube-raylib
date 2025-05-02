#include "Game.h"
#include "GameConfig.h"
#include <memory>


int main()
{
    const char* wd = GetWorkingDirectory();
    GameConfig gameConfig("game.ini");
    Game* gameInstance = &Game::Get();
    gameInstance->Init(gameConfig);
    return gameInstance->Run();
}