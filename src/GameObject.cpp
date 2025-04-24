#include "GameObject.h"
#include "raylib.h"
#include "raymath.h"
#include <typeinfo>


GameObject::GameObject(const char* texturePath, const char* shaderPath, const char* modelPath)
    : m_Texture(LoadTexture(texturePath)), m_Shader(LoadShader(0, shaderPath)), m_Model(LoadModel(modelPath))
{
    m_Material = LoadMaterialDefault();
    transform = MatrixIdentity();
    m_Material.shader = m_Shader;
    SetMaterialTexture(&m_Material, MATERIAL_MAP_DIFFUSE, m_Texture);
}

GameObject::~GameObject()
{
    UnloadModel(m_Model);
    UnloadTexture(m_Texture);
    UnloadMaterial(m_Material);
}

void GameObject::Render()
{
    TraceLog(LOG_WARNING, "Subclasses should implement their own Render method");
}

void GameObject::Update(float deltaTime)
{
    TraceLog(LOG_WARNING, "Subclasses should implement their own Update method");
}

void GameObject::SetPosition(Vector3 position)
{
    TraceLog(LOG_INFO, "Subclasses should implement their own Update method");
}

Vector3 GameObject::GetPosition()
{
    return Vector3{transform.m12, transform.m13, transform.m14};
}

Vector3 GameObject::GetVelocity(float deltaTime, size_t begin, size_t end)
{
    if (m_Positions.size() == 0)
    {
        return Vector3();
    }

    Vector3 firstPosition = m_Positions.at(begin);
    Vector3 lastPosition = m_Positions.at(std::min(end, m_Positions.size()) - 1);
    Vector3 velocity = Vector3Scale(Vector3Subtract(lastPosition, firstPosition), 1.0f / deltaTime);

    return velocity;
}
