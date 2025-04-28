#pragma once
#include "CameraController.h"
#include "GameConfig.h"
#include "GameObject.h"
#include "Utilities.h"
#include "Constants.h"
#include "PhysicsObserver.h"
#include <vector>
#include <map>


class MrAngryCube;

enum class GameState
{
    Playing,
    Paused
};

class Game {
public:
    std::vector<GameObject*> gameObjects;
    std::map<std::string, Shader> shaders;
    std::map<std::string, Texture> textures;
    std::map<std::string, Model> models;
    std::map<std::string, Material> materials;

    Vector3 currentRotationAxis = DEFAULT_NEXT_ROTATION_AXIS;
    Vector3 nextRotationAxis = DEFAULT_NEXT_ROTATION_AXIS;

    CameraController cameraController;
    GameState gameState = GameState::Paused;
    GameConfig* gameConfig;
    PhysicsObserver* physicsObserver;
    
    ~Game();
    static Game& Get();
    void Init(GameConfig& gameConfig);
    void Register(GameObject* newGameObject);
    void Unregister(GameObject* gameObject);
    MrAngryCube* GetPlayer();
    void Render();
    void Update();
    void HandleKeyEvents();
    int Run();
    void Exit();
    
    Game(const Game&) = delete;
    void operator=(const Game&) = delete;
    
private:
    Game();
    MrAngryCube* m_Player;
    
    bool m_Initialized = false;
    float m_LastUpdateTime = 0.0f;
    float m_deltaTime = 0.0f;
};
