#include "GameMode.h"


GameMode::GameMode()
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
    case SpawnBehaviourType::DevtestSpawnBehaviour:
        spawnBehaviour = [this]() {

            };
    default:
        break;
    }
}