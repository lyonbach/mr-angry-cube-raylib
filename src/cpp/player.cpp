#include "player.hpp"
#include "utils.hpp"
#include "game.hpp"
#include <iostream>

MrAngryCubeMoveBehaviour::MrAngryCubeMoveBehaviour(MrAngryCube* p) : player(p) {
    Game& game = Game::getInstance();
    velocity = game.settings["player"]["velocity"];
    waitTime = game.settings["player"]["wait_time"];
    
    heading = Heading::EAST;
    nextHeading = Heading::EAST;
    lastMoveTime = GetTime();
    UpdatePivotPoint(nextHeading);
}

void MrAngryCubeMoveBehaviour::UpdatePivotPoint(Vector3 h) {
    Vector3 pos = player->GetPosition();
    Vector3 pivot = { -pos.x, -pos.y, -pos.z };
    
    Vector3 offset = { -player->size / 2.0f, 0.0f, -player->size / 2.0f };
    // offset = offset * h (manually multiply if not helper)
    Vector3 offsetScaled = { offset.x * h.x + offset.y * h.y + offset.z * h.z, 0, 0 }; // This was wrong in python's V3 multiplication?
    // Wait, Python: raylib.vector3_multiply(get_v3(x=-self.player.size/2, z=-self.player.size/2), heading)
    // In pyray, vector3_multiply is component-wise.
    
    Vector3 componentWiseOffset = {
        (-player->size / 2.0f) * h.x,
        0.0f * h.y,
        (-player->size / 2.0f) * h.z
    };
    
    Vector3 finalOffset = {
        componentWiseOffset.x,
        player->size / 2.0f,
        componentWiseOffset.z
    };
    
    pivotPoint = Vector3Add(pivot, finalOffset);
}

Vector3 MrAngryCubeMoveBehaviour::GetRotationVector() {
    return Vector3CrossProduct({0, 1, 0}, heading);
}

void MrAngryCubeMoveBehaviour::Update() {
    if (heading.x == 0 && heading.y == 0 && heading.z == 0) {
        heading = nextHeading;
        return;
    }

    float angleStep = GetFrameTime() * velocity;

    if (GetTime() - lastMoveTime >= waitTime) {
        canMove = true;
    } else {
        return;
    }

    if (quarterRotation + angleStep - 90.0f > 0) {
        canMove = false;
        quarterRotation = 0;
        player->transform = SnapMatrixToGrid(player->transform);
        lastMoveTime = GetTime();
        heading = nextHeading;
        UpdatePivotPoint(heading);
    } else {
        quarterRotation += angleStep;
    }

    if (canMove) {
        Matrix initialTransform = MatrixTranslate(pivotPoint.x, pivotPoint.y, pivotPoint.z);
        player->transform = MatrixMultiply(player->transform, initialTransform);

        Matrix matRot = MatrixRotate(GetRotationVector(), angleStep * DEG2RAD);
        player->transform = MatrixMultiply(player->transform, matRot);

        Matrix finalTransform = MatrixTranslate(-pivotPoint.x, -pivotPoint.y, -pivotPoint.z);
        player->transform = MatrixMultiply(player->transform, finalTransform);
    }
}

MrAngryCube::MrAngryCube(Vector3 position, float s) : size(s), behaviour(this) {
    transform = MatrixIdentity();
    SetPosition(position);

    Game& game = Game::getInstance();
    auto& assets = game.settings["assets"]["player"];

    model = LoadModel(assets["model_path"].get<std::string>().c_str());
    
    Shader bodyShader = LoadShader(assets["body_vs"].get<std::string>().c_str(), assets["body_fs"].get<std::string>().c_str());
    Shader faceShader = LoadShader(assets["face_vs"].get<std::string>().c_str(), assets["face_fs"].get<std::string>().c_str());

    Texture2D textureBody = LoadTexture(assets["body_tex"].get<std::string>().c_str());
    matBody = LoadMaterialDefault();
    matBody.shader = bodyShader;
    matBody.maps[MATERIAL_MAP_DIFFUSE].texture = textureBody;

    Texture2D textureFace = LoadTexture(assets["face_tex"].get<std::string>().c_str());
    matFace = LoadMaterialDefault();
    matFace.shader = faceShader;
    matFace.maps[MATERIAL_MAP_DIFFUSE].texture = textureFace;

    animation = new Animation(matFace.shader, textureFace, 9);
}

MrAngryCube::~MrAngryCube() {
    UnloadModel(model);
    delete animation;
}

Vector3 MrAngryCube::GetPosition() {
    return { transform.m12, transform.m13, transform.m14 };
}

void MrAngryCube::SetPosition(Vector3 pos) {
    transform.m12 = pos.x;
    transform.m13 = pos.y;
    transform.m14 = pos.z;
}

void MrAngryCube::Update() {
    behaviour.Update();
    animation->Update();
}

void MrAngryCube::Draw() {
    DrawMesh(model.meshes[0], matBody, transform);
    DrawMesh(model.meshes[1], matFace, transform);
}
