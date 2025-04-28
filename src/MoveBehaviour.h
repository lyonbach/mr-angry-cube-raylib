#pragma once

#include "Utilities.h"
#include "GameObject.h"


enum class MoveBehaviourName
{
    NormalMoveBehaviour,
    MoveBehaviourAngerLevel1,
    MoveBehaviourAngerLevel2,
    MoveBehaviourAngerLevel3,
    MoveBehaviourAngerLevel4,
    MoveBehaviourAngerLevelInsane,
    IdleMoveBehaviour
};


class MACMoveBehaviourBase
{
public:
    MACMoveBehaviourBase() = default;
    ~MACMoveBehaviourBase() = default;
    virtual void Action(GameObject* gameObject) = 0;
};


class MACMoveBehaviour : public MACMoveBehaviourBase
{
public:
    float timeUntilNextUpdate = .3f;
    float moveSpeed = 1.0f;
    void Action(GameObject* gameObject) override;
private:
    float m_LastSetCanMoveTime = 0.0f;
};

class MACIdleMoveBehaviour : public MACMoveBehaviour
{
public:
    float moveSpeed = 0.0f;
    float timeUntilNextUpdate = .0f;
    void Action(GameObject* gameObject) override;
};
