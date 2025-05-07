#include "GameObject.h"
#include "Utilities.h"


GameObject::GameObject(Model* model, Material* material, Texture* texture)
    : model(model), material(material), texture(texture)
{
    objectId = Utilities::GenerateHash();
    SetMaterialTexture(material, MATERIAL_MAP_DIFFUSE, *texture);
}

GameObject::~GameObject()
{
    if (model != nullptr)
    {
        // UnloadModel(*model);  // FIXME
        // model = nullptr;  // FIXME
    }
    if (material != nullptr)
    {
        // UnloadMaterial(*material);  // FIXME
        // material = nullptr;  // FIXME
    }
    if (texture != nullptr)
    {
        // UnloadTexture(*texture);  // FIXME
        // texture = nullptr;  // FIXME
    }
}

Vector3
GameObject::GetPosition()
{
    return Vector3{transform.m12, transform.m13, transform.m14};
}

void
GameObject::SetPosition(Vector3 newPosition)
{
    transform.m12 = newPosition.x;
    transform.m13 = newPosition.y;
    transform.m14 = newPosition.z;
}

void
GameObject::Render()
{
    TraceLog(LOG_ERROR, "GameObject subclass should implement its own unique render method!");
}

void
GameObject::Update(float deltaTime)
{
    TraceLog(LOG_ERROR, "GameObject subclass should implement its own unique update method!");
}
