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
    m_AngerControlBehaviour = new NormalAngerControlBehaviour();
}
MrAngryCube::~MrAngryCube()
{
    GameObject::~GameObject();
    delete m_MoveBehaviour;
    delete m_AngerControlBehaviour;
}

void MrAngryCube::Render()
{
    DrawMesh(model->meshes[0], *material, transform);
}

void MrAngryCube::Update(float deltaTime)
{
    m_AngerControlBehaviour->Update();
    m_MoveBehaviour->Action(this);
    if (m_AngerControlBehaviour->anger >= MAXIMUM_ANGER)
    {
        auto allBehaviours = MoveBehaviour::GetAllBehaviourNames();
        int idx = std::find(allBehaviours.begin(), allBehaviours.end(), currentMoveBehaviourName) - allBehaviours.begin();
        SetMoveBehaviour(allBehaviours[(idx + 1) % allBehaviours.size()]);
        m_AngerControlBehaviour->anger = MINIMUM_ANGER;
    }

    if (IsAtQuarterRotation(rotation) && nextMoveBehaviourName != MoveBehaviourName::NoMoveBehaviour)
    {
        ApplyMoveBehaviourChange();
    }
}

float MrAngryCube::GetAnger()
{
    return m_AngerControlBehaviour->anger;
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
    Utilities::Log("Setting move behaviour to: " + std::to_string(static_cast<int>(behaviourName)), "MrAngryCube", LOG_INFO);
    nextMoveBehaviourName = behaviourName;
}

void MrAngryCube::ApplyMoveBehaviourChange()
{
    MACMoveBehaviourBase* moveBehaviour = MoveBehaviour::Get(nextMoveBehaviourName);
    if (moveBehaviour == nullptr){ return; }

    Utilities::Log("Applying move behaviour", "MrAngryCube", LOG_INFO);
    delete m_MoveBehaviour;
    m_MoveBehaviour = moveBehaviour;
    currentMoveBehaviourName = nextMoveBehaviourName;
    nextMoveBehaviourName = MoveBehaviourName::NoMoveBehaviour;
}
