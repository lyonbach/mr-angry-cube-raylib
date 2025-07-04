#pragma once
#include "CameraController.h"
#include "Constants.h"
#include "GameConfig.h"
#include "GameObject.h"
#include "Hud.h"
#include "Gui.h"
#include "PhysicsObserver.h"
#include "Utilities.h"
#include "Level.h"
#include <vector>
#include <map>


class MrAngryCube;
class Level;

enum class GameState
{
    MainMenu,
    Playing,
    Paused,
    LevelSelection
};

class Game {
public:
    bool shouldRun = true;
    bool shouldUpdate = true;
    bool shouldRender = true;

    std::map<std::string, Shader> shaders;
    std::map<std::string, Texture> textures;
    std::map<std::string, Model> models;
    std::map<std::string, Material> materials;
    std::vector<ScheduledEvent*> timedEvents;

    Vector3 currentRotationAxis = DEFAULT_NEXT_ROTATION_AXIS;
    Vector3 nextRotationAxis = DEFAULT_NEXT_ROTATION_AXIS;

    CameraController cameraController;
    GameState gameState = GameState::MainMenu;
    GameConfig* gameConfig = nullptr;
    PhysicsObserver* physicsObserver = nullptr;
    MainMenu* mainMenu = nullptr;
    PauseMenu* pauseMenu = nullptr;
    LevelMenu* levelMenu = nullptr;
    Level* currentLevel = nullptr;
    Hud* hud = nullptr;

    ~Game();
    static Game& Get();
    void Init(GameConfig& gameConfig);
    void LoadLevel(std::string filePath);
    void UnloadLevel();

    MrAngryCube* GetPlayer();
    void ResetPlayer();
    void Render();
    void HandleGui(Menu* menu);
    void Update();
    void HandleKeyEvents();
    void HandleScheduledEvents();
    int Run();
    void Exit();

    Game(const Game&) = delete;
    void operator=(const Game&) = delete;

private:
    Game();
    MrAngryCube* m_Player = nullptr;

    bool m_Initialized = false;
    float m_LastUpdateTime = 0.0f;
    float m_DeltaTime = 0.0f;
    unsigned int m_Anger = MINIMUM_ANGER;
};
