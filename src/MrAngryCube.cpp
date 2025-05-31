#include "MrAngryCube.h"
#include "Game.h"


MrAngryCube::MrAngryCube(Model* model, std::vector<Material*> materials)
    : GameObject(model, materials)
{
    rotation = {0, 0, 0};
    canMove = true;
    size = 1.0f;
    halfSize =  size * .5f;
    hypotenuse = sqrt(halfSize * halfSize * 2);

    m_MoveBehaviour = new MACMoveBehaviour();
    m_AngerControlBehaviour = new NormalAngerControlBehaviour();
    m_FaceAnimation = new TextureAnimation(&materials[1]->shader, 3, 3);
}

MrAngryCube::~MrAngryCube()
{
    Utilities::Log("Destroying MrAngryCube", "MrAngryCube", LOG_DEBUG);
    delete m_MoveBehaviour;
    delete m_AngerControlBehaviour;
}

void MrAngryCube::Render()
{
    DrawMesh(model->meshes[0], *materials[0], transform);
    DrawMesh(model->meshes[1], *materials[1], transform);
}

void MrAngryCube::Update(float deltaTime)
{
    m_AngerControlBehaviour->Update();
    m_MoveBehaviour->Action(this);
    m_FaceAnimation->Update();
    if (m_AngerControlBehaviour->angerCounter >= MAXIMUM_ANGER)
    {
        auto allBehaviours = MoveBehaviour::GetAllBehaviourNames();
        unsigned int idx = GetMoveBehaviourIndex();
        SetMoveBehaviour(allBehaviours[(idx + 1) % allBehaviours.size()]);
        m_AngerControlBehaviour->angerCounter = MINIMUM_ANGER;

        Utilities::Log("Current move behaviour index: " + std::to_string(idx), "MrAngryCube", LOG_DEBUG);

    }
    
    if (IsAtQuarterRotation(rotation) && nextMoveBehaviourName != MoveBehaviourName::NoMoveBehaviour)
    {
        ApplyMoveBehaviourChange();
    }
}

float MrAngryCube::GetAnger() const
{
    return m_AngerControlBehaviour->angerCounter;
}

unsigned int MrAngryCube::GetMoveBehaviourIndex() const
{
    auto allBehaviours = MoveBehaviour::GetAllBehaviourNames();
    return std::find(allBehaviours.begin(), allBehaviours.end(), currentMoveBehaviourName) - allBehaviours.begin();
}

void MrAngryCube::SetAnger(float newAnger)
{
    m_AngerControlBehaviour->angerCounter = newAnger;
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
    Texture* texture;
    Game& game = Game::Get();
    texture = &game.textures[MoveBehaviour::GetTextureNameFromBehaviour(behaviourName)];
    materials[1]->maps->texture = *texture;
}

void MrAngryCube::ApplyMoveBehaviourChange()
{
    MACMoveBehaviourBase* moveBehaviour = MoveBehaviour::Get(nextMoveBehaviourName);
    if (moveBehaviour == nullptr){ return; }

    Shader* shader = &materials[0]->shader;
    unsigned int idx = GetMoveBehaviourIndex();
    float fIdx = static_cast<float>(idx);
    SetShaderValue(*shader, GetShaderLocation(*shader, "uMoveBehaviourIndex"), &fIdx, SHADER_UNIFORM_FLOAT);

    shader = &materials[1]->shader;
    SetShaderValue(*shader, GetShaderLocation(*shader, "uMoveBehaviourIndex"), &fIdx, SHADER_UNIFORM_FLOAT);

    Utilities::Log("Applying move behaviour", "MrAngryCube", LOG_DEBUG);
    delete m_MoveBehaviour;
    m_MoveBehaviour = moveBehaviour;
    currentMoveBehaviourName = nextMoveBehaviourName;
    nextMoveBehaviourName = MoveBehaviourName::NoMoveBehaviour;
}
