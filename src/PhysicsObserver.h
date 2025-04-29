#pragma once

#include "GameObject.h"
#include "raylib.h"
#include <vector>
#include <map>


class PhysicsObserver
{
public:
    GameObject* observed;
    int positionArraySize = 5;  // We observe last 20 positions to determine physics.
    std::vector<std::pair<Vector3, float>> positionArray;
    void Update();
    Vector3 GetVelocity() const;
};