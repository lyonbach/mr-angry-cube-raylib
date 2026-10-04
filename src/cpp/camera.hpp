#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "raylib.h"
#include "player.hpp"

class FollowCamera {
public:
    FollowCamera(MrAngryCube* player, Vector3 offset = { 5, 5, 5 }, float fovy = 45.0f);
    void Update();
    Camera3D GetCamera() { return camera; }

private:
    MrAngryCube* player;
    Vector3 offset;
    Camera3D camera;
};

#endif
