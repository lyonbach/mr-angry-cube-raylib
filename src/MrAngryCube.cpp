#include "MrAngryCube.h"
#include "Game.h"

Vector2 VecSin(Vector2 vec) {
    return (Vector2){sin(DEG2RAD * (45 + abs((int)vec.x) % 90)), sin(DEG2RAD * (45 + abs((int)vec.y % 90)))};
}

void NormalMoveBehaviour(MrAngryCube* player)
{
    // Update cube rotation. We basically calculate the cube vertical displacement and
    // update a 2d vector. We first divide the vector to half cube size then can multiply
    // the x and y values of the vector to update the cube vertical position.
    Vector3& rotation = player->rotation;
    Vector3& rotationAxis = Game::Get().currentRotationAxis;
    rotation = Vector3Add(rotation, Vector3Scale(rotationAxis, player->moveSpeed));

    // Calculate the vertical position of the cube, x stands for the rotation around x axis
    // and y stands for the rotation around z axis. We do not need the other axis since
    // rotation around y does not change the vertical position of the cube.
    // We can use the 2d vector to calculate the vertical position of the cube.
    Vector2 deltaY = Vector2Scale(Vector2Scale(VecSin((Vector2){rotation.x, rotation.z}), player->hypotenuse), 1 / player->halfSize);

    Vector3 incrementVector = Vector3Scale(rotationAxis, DEG2RAD * player->moveSpeed);
    player->transform = MatrixMultiply(
        player->transform, MatrixRotateXYZ((Vector3){incrementVector.x, incrementVector.y, incrementVector.z})
    );

    player->transform.m12 = -rotation.z / 90.0f * player->size * 2;
    player->transform.m13 = deltaY.y * deltaY.x * player->size;
    player->transform.m14 = rotation.x / 90.0f  * player->size * 2;
}

MrAngryCube::MrAngryCube(Model* model, Material* material, Texture* texture)
    : GameObject(model, material, texture)
{
    size = 1.0f;
    halfSize =  size * .5f;
    hypotenuse = sqrt(halfSize * halfSize * 2);
    moveSpeed = 1.0f;

}

void MrAngryCube::Render()
{
    DrawMesh(model->meshes[0], *material, transform);
}

void MrAngryCube::Update(float deltaTime)
{
    NormalMoveBehaviour(this);
}



