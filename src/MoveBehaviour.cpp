#include "MoveBehaviour.h"
#include "Game.h"
#include "MrAngryCube.h"

Vector2 VecSin(Vector2 vec) {
    return (Vector2){sin(DEG2RAD * (45 + abs((int)vec.x) % 90)), sin(DEG2RAD * (45 + abs((int)vec.y % 90)))};
}

void QuantizeVector3(Vector3& vector)
{
    vector.x = round(vector.x * 2) / 2;
    vector.y  = round(vector.y * 2) / 2;
    vector.z = round(vector.z * 2) / 2;
}

void MACMoveBehaviour::Action(GameObject* gameObject)
{

    MrAngryCube* player = (MrAngryCube*)gameObject;
    if (!player)
    {
        Utilities::Log("Player object is null in MACNormalMoveBehaviour::Action", "MoveBehaviour", LOG_FATAL);
        return;
    }

    // Update cube rotation. We basically calculate the cube vertical displacement and
    // update a 2d vector. We first divide the vector to half cube size then can multiply
    // the x and y values of the vector to update the cube vertical position.
    Vector3& rotationAxis = Game::Get().currentRotationAxis;
    Vector3 nextRotation = Vector3Add(player->rotation, Vector3Scale(rotationAxis, moveSpeed));

    if (player->IsAtQuarterRotation(player->rotation) && player->canMove)
    {
        player->canMove = false;
        QuantizeVector3(nextRotation);
        m_LastSetCanMoveTime = GetTime();
    }

    if (!player->canMove)
    {
        if (GetTime() - m_LastSetCanMoveTime < timeUntilNextUpdate) { return; } 
        player->canMove = true;
    }

    player->rotation = nextRotation;

    // Calculate the vertical position of the cube, x stands for the rotation around x axis
    // and y stands for the rotation around z axis. We do not need the other axis since
    // rotation around y does not change the vertical position of the cube.
    // We can use the 2d vector to calculate the vertical position of the cube.
    Vector3 incrementVector = Vector3Scale(rotationAxis, DEG2RAD * moveSpeed);
    Matrix nextTransform = MatrixMultiply(
        player->transform, MatrixRotateXYZ({incrementVector.x, incrementVector.y, incrementVector.z})
    );

    Vector2 deltaY = Vector2Scale(Vector2Scale(VecSin({nextRotation.x, nextRotation.z}), player->hypotenuse), 1 / player->halfSize);
    nextTransform.m12 = -nextRotation.z / 90.0f * player->size * 2;
    nextTransform.m13 = deltaY.y * deltaY.x * player->size;
    nextTransform.m14 = nextRotation.x / 90.0f  * player->size * 2;
    player->transform = nextTransform;
}

void MACNoRotateMoveBehaviour::Action(GameObject* gameObject)
{
    MrAngryCube* player = (MrAngryCube*)gameObject;
    if (!player)
    {
        Utilities::Log("Player object is null in MACNormalMoveBehaviour::Action", "MoveBehaviour", LOG_FATAL);
        return;
    }

    Vector3& rotationAxis = Game::Get().currentRotationAxis;
    Vector3 nextRotation = Vector3Add(player->rotation, Vector3Scale(rotationAxis, moveSpeed));

    if (player->canMove)
    {
        player->canMove = false;
        QuantizeVector3(nextRotation);
        m_LastSetCanMoveTime = GetTime();
    }

    if (!player->canMove)
    {
        if (GetTime() - m_LastSetCanMoveTime < timeUntilNextUpdate) { return; } 
        player->canMove = true;
    }

    player->rotation = nextRotation;


    Vector3 incrementVector = Vector3Scale(rotationAxis, DEG2RAD * moveSpeed);
    Matrix nextTransform = player->transform;

    Vector2 deltaY = Vector2Scale(Vector2Scale(VecSin({nextRotation.x, nextRotation.z}), player->hypotenuse), 1 / player->halfSize);
    nextTransform.m12 = -nextRotation.z / 90.0f * player->size * 2;
    // nextTransform.m13 = deltaY.y * deltaY.x * player->size;
    nextTransform.m14 = nextRotation.x / 90.0f  * player->size * 2;
    player->transform = nextTransform;
}

MACMoveBehaviourBase* MoveBehaviour::Get(MoveBehaviourName name)
{
    if (name == MoveBehaviourName::NormalMoveBehaviour)
        return new MACMoveBehaviour();
    else if (name == MoveBehaviourName::MoveBehaviourAngerLevel1)
    {
        return new MACMoveBehaviourAngerLevel1();
    }
    else if (name == MoveBehaviourName::MoveBehaviourAngerLevel2)
    {
        return new MACMoveBehaviourAngerLevel2();
    }
    else if (name == MoveBehaviourName::MoveBehaviourAngerLevel3)
    {
        return new MACMoveBehaviourAngerLevel3();
    }
    else if (name == MoveBehaviourName::MoveBehaviourAngerLevel4)
    {
        return new MACMoveBehaviourAngerLevel4();
    } else if (name == MoveBehaviourName::MoveBehaviourAngerLevelInsane)
    {
        return new MACMoveBehaviourAngerLevel5();
    } else if (name == MoveBehaviourName::NoRotateMoveBehaviour)
    {
        return new MACNoRotateMoveBehaviour();
    }

    return nullptr;

}

const std::vector<MoveBehaviourName> MoveBehaviour::GetAllBehaviourNames()
{
    return {
        MoveBehaviourName::NoMoveBehaviour,
        MoveBehaviourName::NormalMoveBehaviour,
        MoveBehaviourName::MoveBehaviourAngerLevel1,
        MoveBehaviourName::MoveBehaviourAngerLevel2,
        MoveBehaviourName::MoveBehaviourAngerLevel3,
        MoveBehaviourName::MoveBehaviourAngerLevel4,
        MoveBehaviourName::MoveBehaviourAngerLevelInsane,
        MoveBehaviourName::NoRotateMoveBehaviour,
        MoveBehaviourName::IdleMoveBehaviour
    };
}

const int MoveBehaviour::MoveBehaviourCount()
{
    return GetAllBehaviourNames().size();
}