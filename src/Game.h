#pragma once
#include "CameraController.h"
#include "GameConfig.h"
#include "GameObject.h"
#include "Utilities.h"
#include <vector>
#include <map>


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

    CameraController cameraController;
    GameState gameState = GameState::Paused;
    GameConfig* gameConfig;

    ~Game();
    static Game& Get();
    void Init(GameConfig& gameConfig);
    void Register(GameObject* newGameObject);
    void Unregister(GameObject* gameObject);
    void Render();
    void Update();
    void HandleKeyEvents();
    int Run();
    void Exit();

    Game(const Game&) = delete;
    void operator=(const Game&) = delete;

private:
    Game();
    bool m_Initialized = false;
    float m_LastUpdateTime = 0.0f;
    float m_deltaTime = 0.0f;
};
