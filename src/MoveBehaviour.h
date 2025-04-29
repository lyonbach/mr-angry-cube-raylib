#pragma once

#include "Utilities.h"
#include "GameObject.h"
#include <functional>


enum class MoveBehaviourName
{
    NoMoveBehaviour,
    NormalMoveBehaviour,
    MoveBehaviourAngerLevel1,
    MoveBehaviourAngerLevel2,
    MoveBehaviourAngerLevel3,
    MoveBehaviourAngerLevel4,
    MoveBehaviourAngerLevelInsane,
    NoRotateMoveBehaviour,
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
protected:
    float m_LastSetCanMoveTime = 0.0f;
};

class MACMoveBehaviourAngerLevel1 : public MACMoveBehaviour
{
public:
    MACMoveBehaviourAngerLevel1() : MACMoveBehaviour()
    {
        moveSpeed = 2.0f;
        timeUntilNextUpdate = .2f;
    }
};

class MACMoveBehaviourAngerLevel2 : public MACMoveBehaviour
{
public:
    MACMoveBehaviourAngerLevel2() : MACMoveBehaviour()
    {
        moveSpeed = 3.0f;
        timeUntilNextUpdate = .15f;
    }
};

class MACMoveBehaviourAngerLevel3 : public MACMoveBehaviour
{
public:
    MACMoveBehaviourAngerLevel3() : MACMoveBehaviour()
    {
        moveSpeed = 4.5f;
        timeUntilNextUpdate = 0.1f;
    }
};

class MACMoveBehaviourAngerLevel4 : public MACMoveBehaviour
{
public:
    MACMoveBehaviourAngerLevel4() : MACMoveBehaviour()
    {
        moveSpeed = 5.0f;
        timeUntilNextUpdate = 0.05f;
    }
};

class MACMoveBehaviourAngerLevel5 : public MACMoveBehaviour
{
public:
    MACMoveBehaviourAngerLevel5() : MACMoveBehaviour()
    {
        moveSpeed = 9.0f;
        timeUntilNextUpdate = 0.001f;
    }
};

class MACNoRotateMoveBehaviour : public MACMoveBehaviour
{
public:
    MACNoRotateMoveBehaviour() : MACMoveBehaviour()
    {
        moveSpeed = 45.0f;
        timeUntilNextUpdate = .1f;
    }
    void Action(GameObject* gameObject) override;
};

namespace MoveBehaviour
{
    MACMoveBehaviourBase* Get(MoveBehaviourName name);
    const std::vector<MoveBehaviourName> GetAllBehaviourNames();
    const int MoveBehaviourCount();
};