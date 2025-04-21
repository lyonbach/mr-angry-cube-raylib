#pragma once
#include <functional>
#include <string>

using f_SpawnBehaviour = std::function<void()>;

enum class SpawnBehaviourType
{
    NoSpawnBehaviour,
    DevTestSpawnBehaviour,
    RandomSpawnBehaviour

};


class SpawnBehaviours
{
public:
    static void NoSpawnBehaviour();
    static void DevTestSpawnBehaviour();
    static void RandomSpawnBehaviour();
};
