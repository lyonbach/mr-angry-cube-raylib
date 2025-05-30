#include "GameObject.h"
#include "Utilities.h"


GameObject::GameObject(Model* model, std::vector<Material*> materials)
    : model(model), materials(materials)
{
    objectId = Utilities::GenerateHash();
}

GameObject::~GameObject()
{
    Utilities::Log("GameObject destroyed ID: " + objectId, "GameObject", LOG_DEBUG);
}

Vector3 GameObject::GetPosition()
{
    return Vector3{transform.m12, transform.m13, transform.m14};
}

void GameObject::SetPosition(Vector3 newPosition)
{
    transform.m12 = newPosition.x;
    transform.m13 = newPosition.y;
    transform.m14 = newPosition.z;
}

void GameObject::Render()
{
    TraceLog(LOG_ERROR, "GameObject subclass should implement its own unique render method!");
}

void GameObject::Update(float deltaTime)
{
    TraceLog(LOG_ERROR, "GameObject subclass should implement its own unique update method!");
}
