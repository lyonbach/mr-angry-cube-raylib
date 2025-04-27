#pragma once

#include "Utilities.h"
#include "GameObject.h"


enum class MoveBehaviourType
{
    NormalMoveBehaviour
};


class MACMoveBehaviour
{
public:
    MACMoveBehaviour() = default;
    ~MACMoveBehaviour() = default;
    virtual void Action(GameObject* gameObject) = 0;
};


class MACNormalMoveBehaviour : public MACMoveBehaviour
{
public:
    float lastSetCanMoveTime = 0.0f;
    float timeUntilNextupdate = 1.0f;
    void Action(GameObject* gameObject) override;
};
