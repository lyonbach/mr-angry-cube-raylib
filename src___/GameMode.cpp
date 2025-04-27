#include "GameMode.h"


void GameMode::SetSpawnBehaviour(SpawnBehaviourType spawnBehaviourType)
{
    switch (spawnBehaviourType)
    {
    case SpawnBehaviourType::NoSpawnBehaviour:
        spawnBehaviour = []() {  };
        break;
    case SpawnBehaviourType::RandomSpawnBehaviour:
        spawnBehaviour = [this]() {
            SpawnBehaviours::RandomSpawnBehaviour();
        };
        break;
        case SpawnBehaviourType::DevTestSpawnBehaviour:
        spawnBehaviour = [this]() {
            SpawnBehaviours::DevTestSpawnBehaviour();
        };
    default:
        break;
    }
}

GameMode::GameMode()
{
    SetSpawnBehaviour(SpawnBehaviourType::DevTestSpawnBehaviour);
}