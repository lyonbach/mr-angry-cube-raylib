#pragma once
#include <functional>
#include <string>

using f_SpawnBehaviour = std::function<void()>;

enum class SpawnBehaviourType
{
    NoSpawnBehaviour,
    DevtestSpawnBehaviour,
    RandomSpawnBehaviour

};


class SpawnBehaviours
{
public:
    static void NoSpawnBehaviour();
    static void DevtestSpawnBehaviour();
    static void RandomSpawnBehaviour();
};
