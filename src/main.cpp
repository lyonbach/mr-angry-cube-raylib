#include "Game.h"
#include <memory>

int main()
{
    #ifdef LOG_LEVEL
        TraceLog(LOG_INFO, "Setting log level to %i", LOG_LEVEL);
        SetTraceLogLevel(LOG_LEVEL);
    #endif

    const char* wd = GetWorkingDirectory();
    GameConfig gameConfig("game.ini");
    Game::Get().Init(gameConfig);
    return Game::Get().Run();
}