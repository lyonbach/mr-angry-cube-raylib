#include "MoveBehaviour.h"
#include "Game.h"
#include "MrAngryCube.h"

Vector2 VecSin(Vector2 vec) {
    return (Vector2){sin(DEG2RAD * (45 + abs((int)vec.x) % 90)), sin(DEG2RAD * (45 + abs((int)vec.y % 90)))};
}

void QuantizeVector3(Vector3& vector)
{
    vector.x = round(vector.x * 2) / 2;
    vector.y  = round(vector.y * 2) / 2;
    vector.z = round(vector.z * 2) / 2;
}

void MACNormalMoveBehaviour::Action(GameObject* gameObject)
{

    MrAngryCube* player = (MrAngryCube*)gameObject;
    if (!player)
    {
        Utilities::Log("Player object is null in MACNormalMoveBehaviour::Action", "MoveBehaviour", LOG_FATAL);
        return;
    }

    // Update cube rotation. We basically calculate the cube vertical displacement and
    // update a 2d vector. We first divide the vector to half cube size then can multiply
    // the x and y values of the vector to update the cube vertical position.
    Vector3& rotationAxis = Game::Get().currentRotationAxis;
    Vector3 nextRotation = Vector3Add(player->rotation, Vector3Scale(rotationAxis, player->moveSpeed));

    if (player->IsAtQuarterRotation(player->rotation) && player->canMove)
    {
        player->canMove = false;
        QuantizeVector3(nextRotation);
        lastSetCanMoveTime = GetTime();
    }

    if (!player->canMove)
    {
        if (GetTime() - lastSetCanMoveTime >= timeUntilNextupdate) {
            player->canMove = true;
        } else {
            return;
        }
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