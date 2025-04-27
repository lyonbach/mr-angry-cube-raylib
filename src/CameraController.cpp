#include "CameraController.h"


CameraController::CameraController()
{
    camera = new Camera();
    camera->target = Vector3();
    camera->position = Vector3{ 0.0f, 2.0f, 2.0f };
    camera->up = Vector3{ 0.0f, 1.0f, 0.0f };
    camera->fovy = 70.0f;
    camera->projection = CAMERA_PERSPECTIVE;
}

CameraController::~CameraController()
{
    delete camera;
}