#include "camera.hpp"
#include "raymath.h"

FollowCamera::FollowCamera(MrAngryCube* p, Vector3 off, float fovy) : player(p), offset(off) {
    camera.position = { 0, 0, 0 };
    camera.target = { 0, 0, 0 };
    camera.up = { 0, 1, 0 };
    camera.fovy = fovy;
    camera.projection = CAMERA_PERSPECTIVE;

    camera.target = player->GetPosition();
    camera.position = Vector3Add(camera.target, offset);
}

void FollowCamera::Update() {
    Vector3 playerPos = player->GetPosition();
    Vector3 targetPos = Vector3Add(playerPos, offset);
    
    Vector3 diff = Vector3Subtract(targetPos, camera.position);
    camera.position = Vector3Add(camera.position, Vector3Scale(diff, 0.025f));
    
    camera.position.y = playerPos.y + offset.y;
    camera.target = playerPos;
}
