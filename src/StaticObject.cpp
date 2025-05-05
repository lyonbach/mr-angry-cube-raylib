#include "StaticObject.h"

StaticObject::StaticObject(Model* model, Material* material, Texture* texture)
    : GameObject(model, material, texture) {}

void StaticObject::Render()
{
    DrawMesh(model->meshes[0], *material, transform);
}


void StaticObject::Update(float deltaTime)
{
    // Do nothing...
}