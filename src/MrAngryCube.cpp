#include "MrAngryCube.h"

#include "raylib.h"
#include "raymath.h"


Vector2 VecSin(Vector2 vec) {
    return (Vector2){sin(DEG2RAD * (45 + abs((int)vec.x) % 90)), sin(DEG2RAD * (45 + abs((int)vec.y % 90)))};
}


MrAngryCube::MrAngryCube(const char* texturePath, const char* shaderPath, const char* modelPath)
: GameObject(texturePath, shaderPath, modelPath) {
    m_Rotation = { 0.0f, 0.0f, 0.0f };
    m_Size = 1.0f;
    m_HalfSize = m_Size / 2.0f;
    m_Hypotenuse = sqrt(m_HalfSize * m_HalfSize * 2);
}


void MrAngryCube::Render()
{
    DrawMesh(m_Model.meshes[0], m_Material, m_Transform);
}

void MrAngryCube::Update(Vector3 rotationAxis, float increment)
{
    // Update cube rotation. We basically calculate the cube vertical displacement and
    // update a 2d vector. We first divide the vector to half cube size then can multiply
    // the x and y values of the vector to update the cube vertical position. 
    m_Rotation = Vector3Add(m_Rotation, Vector3Scale(rotationAxis, increment));

    // Calculate the vertical position of the cube, x stands for the rotation around x axis
    // and y stands for the rotation around z axis. We do not need the other axis since 
    // rotation around y does not change the vertical position of the cube. 
    // We can use the 2d vector to calculate the vertical position of the cube.
    Vector2 deltaY = Vector2Scale(Vector2Scale(VecSin((Vector2){m_Rotation.x, m_Rotation.z}), m_Hypotenuse), 1 / m_HalfSize);

    Vector3 incrementVector = Vector3Scale(rotationAxis, DEG2RAD * increment);
    m_Transform = MatrixMultiply(m_Transform, MatrixRotateXYZ((Vector3){incrementVector.x, incrementVector.y, incrementVector.z}));

    m_Transform.m12 = -m_Rotation.z / 90.0f * m_Size * 2;
    m_Transform.m13 = deltaY.y * deltaY.x * m_Size;
    m_Transform.m14 = m_Rotation.x / 90.0f * m_Size * 2;
}