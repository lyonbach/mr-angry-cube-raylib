#include "Enemy.h"
#include "Miscellaneous.h"
#include "MrAngryCube.h"
#include "raylib.h"
#include "raymath.h"
#include <thread>
#include <string>


Vector2 VecSin(Vector2 vec) {
    return (Vector2){sin(DEG2RAD * (45 + abs((int)vec.x) % 90)), sin(DEG2RAD * (45 + abs((int)vec.y % 90)))};
}

MrAngryCube::MrAngryCube(const char* texturePath, const char* shaderPath, const char* modelPath)
: GameObject(texturePath, shaderPath, modelPath) {
    rotation = { 0.0f, 0.0f, 0.0f };
    m_Size = 1.0f;
    m_HalfSize = m_Size / 2.0f;
    m_Hypotenuse = sqrt(m_HalfSize * m_HalfSize * 2);
}

void MrAngryCube::Render()
{
    DrawMesh(m_Model.meshes[0], m_Material, transform);
}

void MrAngryCube::Update(float deltaTime)
{
    if(!m_IsMoving) {
        return;
    }

    // Update cube rotation. We basically calculate the cube vertical displacement and
    // update a 2d vector. We first divide the vector to half cube size then can multiply
    // the x and y values of the vector to update the cube vertical position.
    float scaledSpeed = speed * gameInfo.anger;
    rotation = Vector3Add(rotation, Vector3Scale(rotationAxis, scaledSpeed));

    // Calculate the vertical position of the cube, x stands for the rotation around x axis
    // and y stands for the rotation around z axis. We do not need the other axis since
    // rotation around y does not change the vertical position of the cube.
    // We can use the 2d vector to calculate the vertical position of the cube.
    Vector2 deltaY = Vector2Scale(Vector2Scale(VecSin((Vector2){rotation.x, rotation.z}), m_Hypotenuse), 1 / m_HalfSize);

    Vector3 incrementVector = Vector3Scale(rotationAxis, DEG2RAD * scaledSpeed);
    transform = MatrixMultiply(transform, MatrixRotateXYZ((Vector3){incrementVector.x, incrementVector.y, incrementVector.z}));

    transform.m12 = -rotation.z / 90.0f * m_Size * 2;
    transform.m13 = deltaY.y * deltaY.x * m_Size;
    transform.m14 = rotation.x / 90.0f * m_Size * 2;

    if(IsAtQuarterRotation())
    {
        bool shouldIncreaseAnger = false;
        const char* quote = "";

        rotationCount += Utilities::AbsVector3(rotationAxis);
        int rotationCountSum = Utilities::SumVector3(rotationCount);
        if (rotationCountSum % 10 == 0 && rotationCountSum > 0)
        {
            quote = "Dizzy and angry!!!";  // FIXME GET RANDOM QUOTE FROM A BUNCH OF QUOTES.
            shouldIncreaseAnger = true;
        }

        if (IsFaceOnTheGround())  // Handling when face hits the ground.
        {
            quote = "My Face!\n No more Mr. Nice Cube!";
            shouldIncreaseAnger = true;

            // Create a text to be displayed. FIXME THIS LOGIC WILL BE CHANGED TO SHOW SOME TEXTURE OR DECAL.
            WaitForNonBlocking(m_MovePauseDuration * 3);
        } else {
            WaitForNonBlocking(m_MovePauseDuration);
        }

        if (shouldIncreaseAnger)
        {
            gameInfo.anger = std::min(gameInfo.maxAnger, gameInfo.anger + 0.5f);
        }

        if (quote != "")
        {
            TimedText* timedText = Utilities::GetTimedText(quote);
            timedText->lastCheckTime = GetTime();
            timedTexts.push_back(timedText);
        }

        rotationAxis = nextRotationAxis;
    }
}

bool MrAngryCube::IsFaceOnTheGround()
{
    if (!IsAtQuarterRotation())
    {
        return false;
    }
    // Mr. Angry Cube gets more angry when he hits his face on the ground.
    Vector3 down = { 0.0f, 1.0f, 0.0f };  // Global down vector.
    Vector3 cubeUp = Vector3Transform(down, transform);
    float dotProduct = Vector3DotProduct(cubeUp, down);

    // If the cube's "down" vector is not aligned with the world's "down" vector, it means a face is on the ground.
    return fabs(dotProduct) < 0.1f;
}

bool MrAngryCube::IsAtQuarterRotation(bool ommitZero)
{
    bool result = (
        (int)rotation.x % 90 == 0 && rotationAxis.x != 0.0f ||
        (int)rotation.z % 90 == 0 && rotationAxis.z != 0.0f ||
        (int)rotation.y % 90 == 0 && rotationAxis.y != 0.0f ||
        (rotationAxis.x == 0.0f &&
         rotationAxis.z == 0.0f &&
         rotationAxis.y == 0.0f));
    if (!ommitZero)
    {
        result = result && (rotation.x != 0 || rotation.z != 0);
    }
    return result;
}

const void MrAngryCube::WaitForNonBlocking(float seconds)
{
    m_IsMoving = false;
    std::thread([this, seconds]() {
        std::this_thread::sleep_for(std::chrono::duration<float>(seconds));
        m_IsMoving = true;
    }).detach();
}