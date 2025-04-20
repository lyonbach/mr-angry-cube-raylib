#pragma once
#include "Behaviours.h"
#include "raylib.h"
#include <filesystem>


class GameMode
{
public:
    GameMode();
    SpawnBehaviourType spawnBehaviourType = SpawnBehaviourType::RandomSpawnBehaviour;
    f_SpawnBehaviour spawnBehaviour;
};
