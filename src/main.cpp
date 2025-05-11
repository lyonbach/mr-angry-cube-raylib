#include "Game.h"
// #include "raylib.h"
#include <memory>

int main()
{
    #ifdef LOG_LEVEL
        TraceLog(LOG_ALL, "Setting log level to %i", LOG_LEVEL);
        SetTraceLogLevel(LOG_LEVEL);
    #endif

    const char* wd = GetWorkingDirectory();
    GameConfig gameConfig("game.ini");
    Game* gameInstance = &Game::Get();
    gameInstance->Init(gameConfig);
    return gameInstance->Run();
}