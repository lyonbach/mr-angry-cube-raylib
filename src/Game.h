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

    GameConfig* gameConfig;
    CameraController cameraController;
    GameState gameState = GameState::Paused;

    static Game& Get();
    ~Game();
    void Init(GameConfig* gameConfig);
    void Register(GameObject* newGameObject);
    void Unregister(GameObject* gameObject);
    void Render();
    void Update(float lastLoopTime);
    void HandleKeyEvents();
    int Run();
    void Exit();
    
    Game(const Game&) = delete;
    void operator=(const Game&) = delete;

private:
    Game();
    bool m_Initialized = false;
};
