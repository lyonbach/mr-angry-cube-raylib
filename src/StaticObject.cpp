#include "StaticObject.h"

StaticObject::StaticObject(Model* model, std::vector<Material*> materials)
    : GameObject(model, materials) {}

void StaticObject::Render()
{
    DrawMesh(model->meshes[0], *materials[0], transform);
}


void StaticObject::Update(float deltaTime)
{
    // Do nothing...
}