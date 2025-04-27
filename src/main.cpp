#include "Game.h"
#include "GameConfig.h"
#include <memory>


int main()
{
    const char* wd = GetWorkingDirectory();

    float updateRate = 90.0f;
    #ifdef PLATFORM_WEB
        updateRate = 120;
        TraceLog(LOG_INFO, "Setting everything for webplatform...");
    #endif

    // Create game configuration.
    GameConfig gameConfig;

    Game* gameInstance = &Game::Get();
    gameInstance->Init(gameConfig);
    return gameInstance->Run();
}