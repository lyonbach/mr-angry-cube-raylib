#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include "raylib.h"
#include "raymath.h"
#include "game.hpp"
#include <string>

class IGameObject {
public:
    IGameObject(const std::string& name, Vector3 position = {0, 0, 0}, float size = 1.0f)
        : name(name), position(position), size(size) {
        transform = MatrixIdentity();
        // Set position in transform matrix
        transform.m12 = position.x;
        transform.m13 = position.y;
        transform.m14 = position.z;
    }

    virtual ~IGameObject() {}

    virtual void Update() = 0;
    virtual void Draw() = 0;

    Vector3 GetPosition() const {
        return { transform.m12, transform.m13, transform.m14 };
    }

    void SetPosition(Vector3 pos) {
        transform.m12 = pos.x;
        transform.m13 = pos.y;
        transform.m14 = pos.z;
    }

    std::string name;
    Matrix transform;
    float size;

protected:
    Vector3 position;
};

class GameObject : public IGameObject {
public:
    GameObject(const std::string& name, Vector3 position = {0, 0, 0}, float size = 1.0f)
        : IGameObject(name, position, size) {
        
        auto& game = Game::getInstance();
        auto& assets = game.settings["assets"][name];

        model = LoadModel(assets["model_path"].get<std::string>().c_str());
        
        if (assets.contains("main_vs") && assets.contains("main_fs")) {
            shader = LoadShader(assets["main_vs"].get<std::string>().c_str(), assets["main_fs"].get<std::string>().c_str());
        } else {
            shader = LoadShader(0, 0); // Load default shader
        }
        
        if (assets.contains("main_tex")) {
            Texture2D tex = LoadTexture(assets["main_tex"].get<std::string>().c_str());
            model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = tex;
        }
        
        model.materials[0].shader = shader;
    }

    virtual ~GameObject() {
        UnloadModel(model);
        UnloadShader(shader);
    }

    void Update() override {
        // Default implementation
    }

    void Draw() override {
        DrawModel(model, GetPosition(), 1.0f, WHITE); 
        // Note: DrawModel doesn't take Matrix, we should use DrawMesh with transform
        // But for simplicity in base class:
        DrawMesh(model.meshes[0], model.materials[0], transform);
    }

protected:
    Model model;
    Shader shader;
};

#endif
