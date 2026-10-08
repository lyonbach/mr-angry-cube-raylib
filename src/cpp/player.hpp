#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "raylib.h"
#include "raymath.h"
#include "animation.hpp"
#include "game_object.hpp"
#include <string>

struct Heading {
    static inline Vector3 NO_HEADING = {  0.0f, 0.0f,  0.0f };
    static inline Vector3 NORTH      = {  1.0f, 0.0f,  0.0f };
    static inline Vector3 SOUTH      = { -1.0f, 0.0f,  0.0f };
    static inline Vector3 EAST       = {  0.0f, 0.0f,  1.0f };
    static inline Vector3 WEST       = {  0.0f, 0.0f, -1.0f };
};

class MrAngryCube;

class MrAngryCubeMoveBehaviour {
public:
    MrAngryCubeMoveBehaviour(MrAngryCube* player);
    void Update();
    void UpdatePivotPoint(Vector3 heading);
    
    Vector3 heading;
    Vector3 nextHeading;
    bool canMove = true;

private:
    MrAngryCube* player;
    float quarterRotation = 0.0f;
    float waitTime = 0.25f;
    float velocity = 90.0f;
    double lastMoveTime;
    Vector3 pivotPoint;

    Vector3 GetRotationVector();
};

class MrAngryCube : public GameObject {
public:
    MrAngryCube(const std::string& name, Vector3 position, float size = 2.0f);
    ~MrAngryCube();
    
    void Update() override;
    void Draw() override;
    
    MrAngryCubeMoveBehaviour behaviour;

private:
    Material matBody;
    Material matFace;
    Animation* animation;
};

#endif
