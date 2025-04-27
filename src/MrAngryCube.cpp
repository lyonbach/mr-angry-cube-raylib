#include "MrAngryCube.h"
#include "Game.h"


MrAngryCube::MrAngryCube(Model* model, Material* material, Texture* texture)
    : GameObject(model, material, texture){}

void
MrAngryCube::Render()
{
    DrawMesh(model->meshes[0], *material, transform);
}