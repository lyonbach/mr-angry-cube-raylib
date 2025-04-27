#include "MrAngryCube.h"
#include "Game.h"

Vector2 VecSin(Vector2 vec) {
    return (Vector2){sin(DEG2RAD * (45 + abs((int)vec.x) % 90)), sin(DEG2RAD * (45 + abs((int)vec.y % 90)))};
}

void NormalMoveBehaviour(MrAngryCube* player)
{

    if (!player->canMove)
    {
        return;
    }
    // Update cube rotation. We basically calculate the cube vertical displacement and
    // update a 2d vector. We first divide the vector to half cube size then can multiply
    // the x and y values of the vector to update the cube vertical position.
    Vector3& rotationAxis = Game::Get().currentRotationAxis;
    Vector3 nextRotation = Vector3Add(player->rotation, Vector3Scale(rotationAxis, player->moveSpeed));
    
    
    if (player->IsAtQuarterRotation(player->rotation) && player->HasEverMoved())
    {
        // Utilities::Log(player->GetPosition());
        Utilities::Log(player->rotation);
        Utilities::Log(std::to_string(player->HasEverMoved()));
        player->canMove = false;
        return;
    }

    player->rotation = nextRotation;
    
    // Calculate the vertical position of the cube, x stands for the rotation around x axis
    // and y stands for the rotation around z axis. We do not need the other axis since
    // rotation around y does not change the vertical position of the cube.
    // We can use the 2d vector to calculate the vertical position of the cube.
    Vector3 incrementVector = Vector3Scale(rotationAxis, DEG2RAD * player->moveSpeed);
    Matrix nextTransform = MatrixMultiply(
        player->transform, MatrixRotateXYZ({incrementVector.x, incrementVector.y, incrementVector.z})
    );

    Vector2 deltaY = Vector2Scale(Vector2Scale(VecSin({nextRotation.x, nextRotation.z}), player->hypotenuse), 1 / player->halfSize);
    nextTransform.m12 = -nextRotation.z / 90.0f * player->size * 2;
    nextTransform.m13 = deltaY.y * deltaY.x * player->size;
    nextTransform.m14 = nextRotation.x / 90.0f  * player->size * 2;
    player->transform = nextTransform;
}

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
    NormalMoveBehaviour(this);
}



