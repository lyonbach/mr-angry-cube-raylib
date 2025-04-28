#include "MrAngryCube.h"
#include "Game.h"


MrAngryCube::MrAngryCube(Model* model, Material* material, Texture* texture)
    : GameObject(model, material, texture)
{
    rotation = {0, 0, 0};
    canMove = true;
    size = 1.0f;
    halfSize =  size * .5f;
    hypotenuse = sqrt(halfSize * halfSize * 2);
    m_MoveBehaviour = new MACMoveBehaviour();
}

void MrAngryCube::Render()
{
    DrawMesh(model->meshes[0], *material, transform);
}

void MrAngryCube::Update(float deltaTime)
{
    m_MoveBehaviour->Action(this);
    if (IsAtQuarterRotation(rotation))
    {
        ApplyMoveBehaviourChange();
    }
}

bool MrAngryCube::IsAtQuarterRotation(Vector3& rotation) const
{
    bool result = (
        abs(fmod(rotation.x, 90.0f)) < 0.05f &&
        abs(fmod(rotation.y, 90.0f)) < 0.05f &&
        abs(fmod(rotation.z, 90.0f)) < 0.05f
    );
    return result;
}

bool MrAngryCube::HasEverMoved() const
{
    bool result = (
        rotation.x != 0.0f ||
        rotation.y != 0.0f ||
        rotation.z != 0.0f
    );
    return result;
}

void MrAngryCube::SetMoveBehaviour(MoveBehaviourName behaviourName)  // FIXME THIS COULD BE BETTER
{
    Utilities::Log("Setting move behaviour to: " + std::to_string(static_cast<int>(behaviourName)), "MrAngryCube", LOG_DEBUG);
    nextMoveBehaviourName = behaviourName;
}

void MrAngryCube::ApplyMoveBehaviourChange()
{
    switch (nextMoveBehaviourName)
    {
    case MoveBehaviourName::NormalMoveBehaviour:
        m_MoveBehaviour->moveSpeed = 1.0f;
        m_MoveBehaviour->timeUntilNextUpdate = 0.3f;
    break;
    case MoveBehaviourName::MoveBehaviourAngerLevel1:
        m_MoveBehaviour->moveSpeed = 2.0f;
        m_MoveBehaviour->timeUntilNextUpdate = 0.2f;
    break;
    case MoveBehaviourName::MoveBehaviourAngerLevel2:
        m_MoveBehaviour->moveSpeed = 3.0f;
        m_MoveBehaviour->timeUntilNextUpdate = 0.15f;
    break;
    case MoveBehaviourName::MoveBehaviourAngerLevel3:
        m_MoveBehaviour->moveSpeed = 4.5f;
        m_MoveBehaviour->timeUntilNextUpdate = 0.1f;
    break;
    case MoveBehaviourName::MoveBehaviourAngerLevel4:
        m_MoveBehaviour->moveSpeed = 5.0f;
        m_MoveBehaviour->timeUntilNextUpdate = 0.05f;
    break;
    case MoveBehaviourName::MoveBehaviourAngerLevelInsane:
        m_MoveBehaviour->moveSpeed = 9.0f;
        m_MoveBehaviour->timeUntilNextUpdate = 0.001f;
    break;
    }
}
