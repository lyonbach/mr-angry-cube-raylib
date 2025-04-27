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
    moveSpeed = 1.0f;

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

void MrAngryCube::Render()
{
    DrawMesh(model->meshes[0], *material, transform);
}

void MrAngryCube::Update(float deltaTime)
{
    m_MoveBehaviour.Action(this);
}
