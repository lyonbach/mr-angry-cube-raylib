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
    // TraceLog(LOG_DEBUG, "Destroying instance of class: %s", typeid(*this).name());
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
