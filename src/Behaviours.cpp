#include "Behaviours.h"
#include "Enemy.h"
#include "Miscellaneous.h"
#include "Game.h"
#include "raylib.h"


void SpawnBehaviours::NoSpawnBehaviour()
{
    Utilities::Log("No Spawn!", "GameMode", LOG_WARNING);
}

void SpawnBehaviours::DevTestSpawnBehaviour()
{
    for (int i = 0; i < 10; i++)
    {
        RandomSpawnBehaviour();
    }
}

void SpawnBehaviours::RandomSpawnBehaviour()
{
    // Enemies can appear next to the Mr. Angry Cube or 20 unit distance from him.
    int randX = GetRandomValue(2, 20) - 10;
    int randZ = GetRandomValue(2, 20) - 10;

    // In addition, each coordinate should be divisible to 2.
    randX += Game::Get().mrAngryCube->GetPosition().x;
    randZ += Game::Get().mrAngryCube->GetPosition().z;

    if (randX % 2 != 0) randX++;
    if (randZ % 2 != 0) randZ++;

    Enemy* enemy = new Enemy(Game::Get().gameConfig->texturePath.c_str(), Game::Get().gameConfig->shaderPath.c_str(), Game::Get().gameConfig->modelPath.c_str());
    enemy->SetPosition({(float)randX, .5, (float)randZ});
    Game::Get().Register(enemy);
}