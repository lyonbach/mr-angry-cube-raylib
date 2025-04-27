#include "Game.h"
#include "GameConfig.h"
#include <memory>


int main()
{
    const char* wd = GetWorkingDirectory();

    int updateRate = 90;  
    #ifdef PLATFORM_WEB
        updateRate = 120;
        TraceLog(LOG_INFO, "Setting everything for webplatform...");
    #endif

    GameConfig gameConfig({1024, 768}, DARKGRAY);
    gameConfig.updateRate = updateRate;
    Game* gameInstance = &Game::Get();
    gameInstance->Init(&gameConfig);
    unsigned int returnCode = gameInstance->Run();

    return returnCode;
}