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

    Shader& shader = Game::Get().shaders["enemyDefault"];
    Texture& texture = Game::Get().textures["enemyDefault"];
    Model& model = Game::Get().models["enemyDefault"];

    Enemy* enemy = new Enemy(model, shader, texture);
    enemy->SetPosition({(float)randX, 0.0f, (float)randZ});
    Game::Get().Register(enemy);
}