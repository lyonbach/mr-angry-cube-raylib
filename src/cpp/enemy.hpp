#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "game_object.hpp"

class EnemyBase : public GameObject {
public:
    EnemyBase(Vector3 position = {0, 0, 0}, float size = 1.0f)
        : GameObject("enemy", position, size) {}

    void Update() override {
        // Implement enemy movement here
    }

    void Draw() override {
        GameObject::Draw();
    }
};

#endif
