#pragma once
#include "raylib.h"

class CameraController
{
public:
    CameraController();
    ~CameraController();
    void Update(float deltaTime);
    
    Camera* camera;
    Vector3 chaseVector{ 0.0f, 4.0f, 5.0f };
};