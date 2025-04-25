#include "Enemy.h"
#include "Miscellaneous.h"
#include "MrAngryCube.h"
#include "raylib.h"
#include "raymath.h"
#include <string>


Vector2 VecSin(Vector2 vec) {
    return (Vector2){sin(DEG2RAD * (45 + abs((int)vec.x) % 90)), sin(DEG2RAD * (45 + abs((int)vec.y % 90)))};
}

MrAngryCube::MrAngryCube(Model& model, Shader& shader, Texture& texture)
    : GameObject(model, shader, texture)
{
    rotation = { 0.0f, 0.0f, 0.0f };
    m_Size = 1.0f;
    m_HalfSize = m_Size / 2.0f;
    m_Hypotenuse = sqrt(m_HalfSize * m_HalfSize * 2);
    speed = Game::Get().gameInfo.possibleSpeeds.at(0);
}

void MrAngryCube::Render()
{
    DrawMesh(m_Model.meshes[0], m_Material, transform);
}

void MrAngryCube::Update(float deltaTime)
{
    bool shouldGetAngry = false;
    const char* possibleQuote = "";

    // Update velocity array.
    if (m_Positions.size() >= 20)
    {
        m_Positions.erase(m_Positions.begin());
    }
    m_Positions.push_back(GetPosition());

    Game& game = Game::Get();
    if(IsAtQuarterRotation())
    {
        if (IsFaceOnTheGround())  // Handle when we hit the face.
        {
            game.gameInfo.angerIncrementCountdown = game.gameInfo.defaultAngerIncrementCountdown;
            game.gameInfo.faceHits++;
            shouldGetAngry = true;

            if (game.timedTexts.size() <= 0)
            {
                possibleQuote = Utilities::GetQuote(Reason::FaceHit);
                game.timedTexts.push_back(Utilities::GetTimedText(possibleQuote, Reason::FaceHit));
            }
            WaitFor(1.0f);
        } else {
            WaitFor(.3f);
        }

        if(!canMove) { return; }

        // Quantize the rotations so that we won't
        // accidently keep insignificant amounts of rotation.
        rotation.x = round(rotation.x / 90.0f) * 90.0f;
        rotation.y = round(rotation.y / 90.0f) * 90.0f;
        rotation.z = round(rotation.z / 90.0f) * 90.0f;

        // Update rotation count in the game information and decrement anger increment countdown.
        int totalRotationCount = Utilities::SumVector3(game.gameInfo.rotationCount);
        game.gameInfo.angerIncrementCountdown--;
        game.gameInfo.rotationCount += Utilities::AbsVector3(rotationAxis);

        if (game.gameInfo.angerIncrementCountdown <= 0 && totalRotationCount > 0)
        {
            shouldGetAngry = true;
            game.gameInfo.angerIncrementCountdown = game.gameInfo.defaultAngerIncrementCountdown;
            if (game.timedTexts.size() <= 0)
            {
                possibleQuote = Utilities::GetQuote(Reason::Dizzyness);
                game.timedTexts.push_back(Utilities::GetTimedText(possibleQuote, Reason::Dizzyness));
            }
        }

        if (game.gameInfo.anger == game.gameInfo.maxAnger)
        {
            game.gameInfo.gameOverCountdown--;
            if (game.timedTexts.size() <= 0)
            {
                possibleQuote = Utilities::GetQuote(Reason::AlmostGameOver);
                game.timedTexts.push_back(Utilities::GetTimedText(possibleQuote, Reason::AlmostGameOver));
            }
        }

        if (shouldGetAngry)
        {
            game.gameInfo.anger = std::min(game.gameInfo.maxAnger, ++game.gameInfo.anger);
        }

        // // Update all.
        rotationAxis = nextRotationAxis;  // Update rotaion axis.
        game.gameInfo.lastRotationCount = totalRotationCount;  // Update last rotation count.
        speed = game.gameInfo.possibleSpeeds.at(game.gameInfo.anger);

    }

    // Update cube rotation. We basically calculate the cube vertical displacement and
    // update a 2d vector. We first divide the vector to half cube size then can multiply
    // the x and y values of the vector to update the cube vertical position.
    rotation = Vector3Add(rotation, Vector3Scale(rotationAxis, speed));

    // Calculate the vertical position of the cube, x stands for the rotation around x axis
    // and y stands for the rotation around z axis. We do not need the other axis since
    // rotation around y does not change the vertical position of the cube.
    // We can use the 2d vector to calculate the vertical position of the cube.
    Vector2 deltaY = Vector2Scale(Vector2Scale(VecSin((Vector2){rotation.x, rotation.z}), m_Hypotenuse), 1 / m_HalfSize);

    Vector3 incrementVector = Vector3Scale(rotationAxis, DEG2RAD * speed);
    transform = MatrixMultiply(transform, MatrixRotateXYZ((Vector3){incrementVector.x, incrementVector.y, incrementVector.z}));

    transform.m12 = -rotation.z / 90.0f * m_Size * 2;
    transform.m13 = deltaY.y * deltaY.x * m_Size;
    transform.m14 = rotation.x / 90.0f * m_Size * 2;
}

bool MrAngryCube::IsFaceOnTheGround()
{
    if (!IsAtQuarterRotation())
    {
        return false;
    }

    // Mr. Angry Cube gets more angry when he hits his face on the ground.
    Vector3 up = { 0.0f, 1.0f, 0.0f };  // Global down vector.
    Vector3 cubeUp = Vector3Transform(up, transform);
    float dotProduct = Vector3DotProduct(cubeUp, up);

    // If the cube's "up" vector is not aligned with the world's "up" vector, it means a face is on the ground.
    return fabs(dotProduct) < 0.1f;
}

bool MrAngryCube::IsAtQuarterRotation(bool ommitZero)
{
    // Quantize rotations.
    bool result = (
        ((int)rotation.x) % 90 == 0 && rotationAxis.x != 0.0f ||
        ((int)rotation.z) % 90 == 0 && rotationAxis.z != 0.0f ||
        ((int)rotation.y) % 90 == 0 && rotationAxis.y != 0.0f ||
        (rotationAxis.x == 0.0f && rotationAxis.z == 0.0f && rotationAxis.y == 0.0f)
    );

    if (!ommitZero)
    {
        result = result && (rotation.x != 0 || rotation.z != 0);
    }
    return result;
}

void MrAngryCube::WaitFor(float seconds)
{
    if (canMove)
    {
        canMove = false;
        m_LastMovementCheckTime = GetTime();
    } else {
        if(GetTime() - m_LastMovementCheckTime >= seconds)
        {
            canMove = true;
        }
    }
}
