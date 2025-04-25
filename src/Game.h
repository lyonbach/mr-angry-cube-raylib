#pragma once
#include "Behaviours.h"
#include "CameraController.h"
#include "Enemy.h"
#include "GameConfig.h"
#include "GameInfo.h"
#include "GameMode.h"
#include "GameObject.h"
#include "Miscellaneous.h"
#include "MrAngryCube.h"
#include "SimpleGui.h"
#include <vector>
#include <map>


class MrAngryCube;
class CameraController;

class Game {
public:
    GameInfo gameInfo;
    std::vector<TimedText*> timedTexts;
    std::vector<GameObject*> gameObjects;
    std::map<std::string, Shader> shaders;
    std::map<std::string, Texture> textures;
    std::map<std::string, Model> models;

    GameConfig* gameConfig = nullptr;
    MrAngryCube* mrAngryCube = nullptr;

    static Game& Get();
    ~Game();
    void Init(GameConfig* configuration);
    void SpawnEnemy();
    void Register(GameObject* gameObject);
    void Unregister(GameObject* gameObject);
    void InitMenu();
    void Update(float deltaTime);
    void LoadLevel(const char* levelName);
    void Render();
    void RenderHud();
    void HandleKeyEvents();
    int Run();
    void Exit();
    std::vector<Enemy*> GetCollidingEnemies();
    std::vector<Enemy*> GetEnemies();
    
    Game(const Game&) = delete;
    void operator=(const Game&) = delete;

private:
    Game();
    std::vector<TimedText*> m_TimedTexts;
    bool m_Initialized = false;
    Menu* m_Menu = nullptr;
    CameraController m_CamController;
    GameState m_GameState = GameState::MainMenu;
    GameMode m_GameMode;
};
