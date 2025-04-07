#include "GameObject.h"

#include "raylib.h"
#include "raymath.h"

#include <filesystem>


Vector2 VecSin(Vector2 vec) {
    return (Vector2){sin(DEG2RAD * (45 + abs((int)vec.x) % 90)), sin(DEG2RAD * (45 + abs((int)vec.y % 90)))};
}


GameObject::GameObject()
{
    TraceLog(LOG_INFO, "GameObject constructor called");
    std::filesystem::path fs = std::filesystem::path(__FILE__).parent_path();
    std::string texturePathStr = (fs / "../textures" / "texel_checker.png").string();
    std::string shaderPathStr = (fs / "../vendor/raylib/examples/shaders/resources/shaders/glsl330" / "blur.fs").string();
    std::string modelPathStr = (fs / "../models" / "mr_angry_cube.obj").string();

    m_Model = LoadModel(modelPathStr.c_str());
    m_Texture = LoadTexture(texturePathStr.c_str());
    m_Shader = LoadShader(0, shaderPathStr.c_str());
    m_Material = LoadMaterialDefault();

    m_Transform = MatrixIdentity();
    m_Rotation = { 0.0f, 0.0f, 0.0f };

    m_Material.shader = m_Shader;
    SetMaterialTexture(&m_Material, MATERIAL_MAP_DIFFUSE, m_Texture);

    m_Size = 1.0f;
    m_HalfSize = m_Size / 2.0f;
    m_Hypotenuse = sqrt(m_HalfSize * m_HalfSize * 2);

}

GameObject::~GameObject()
{
    UnloadModel(m_Model);
    UnloadTexture(m_Texture);
    UnloadMaterial(m_Material);
}

void GameObject::Render()
{
    DrawMesh(m_Model.meshes[0], m_Material, m_Transform);
}

void GameObject::Update(Vector3 rotationAxis, float increment)
{

    // Update cube rotation. We basically calculate the cube vertical displacement and
    // update a 2d vector. We first divide the vector to half cube size then can multiply
    // the x and y values of the vector to update the cube vertical position. 
    m_Rotation = Vector3Add(m_Rotation, Vector3Scale(rotationAxis, increment));

    // Calculate the vertical position of the cube, x stands for the rotation around x axis
    // and y stands for the rotation around z axis. We do not need the other axis, 
    // we can use 2d vector to calculate the vertical position of the cube.
    Vector2 deltaY = Vector2Scale(Vector2Scale(VecSin((Vector2){m_Rotation.x, m_Rotation.z}), m_Hypotenuse), 1 / m_HalfSize);

    Vector3 incrementVector = Vector3Scale(rotationAxis, DEG2RAD * increment);
    m_Transform = MatrixMultiply(m_Transform, MatrixRotateXYZ((Vector3){incrementVector.x, incrementVector.y, incrementVector.z}));

    m_Transform.m12 = -m_Rotation.z / 90.0f * m_Size * 2;
    m_Transform.m13 = deltaY.y * deltaY.x * m_Size;
    m_Transform.m14 = m_Rotation.x / 90.0f * m_Size * 2;

}