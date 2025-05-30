#include "MrAngryCube.h"
#include "Game.h"
#include "Constants.h"
#include "Gui.h"
#include "StaticObject.h"
#include <stdexcept>

bool AssertLevelLoaded()
{
    return (Game::Get().currentLevel != nullptr); // true = loaded
}
#define LEVEL_LOADED_GUARD if (!AssertLevelLoaded()) { return; }


Game::Game()
{
    Utilities::Log("Created Game singleton class.", "Game");
}

Game::~Game()
{
    delete physicsObserver;
}

Game& Game::Get()
{
    static Game instance;
    return instance;
}

void Game::Init(GameConfig& config)
{
    gameConfig = &config;
    InitWindow(gameConfig->screenSize.x, gameConfig->screenSize.y, "Mr. Angry Cube (DEV)");
    SetExitKey(0);

    if (gameConfig->fullScreen) { ToggleFullscreen(); }

    // Load textures first to be able to set the material textures.
    Utilities::Log("Loading textures...", "Game", LOG_INFO);
    for (std::pair<std::string, std::string> pair : gameConfig->texturePaths) // Textures.
    {
        Utilities::Log("Loading: " + pair.first + " from:\n" + pair.second, "Game");
        Texture texture = LoadTexture(pair.second.c_str());
        if (!texture.id)
        {
            throw std::runtime_error("Failed to load texture: " + pair.first + " from path: " + pair.second);
        }
        textures[pair.first] = texture;
    }

    // Initialize models, shaders, textures and materials.
    Utilities::Log("Loading models...", "Game", LOG_DEBUG);
    for (std::pair<std::string, std::string> pair : gameConfig->modelPaths)
    {
        Utilities::Log("Loading: " + pair.first + " from:\n" + pair.second, "Game");
        Model model = LoadModel(pair.second.c_str());
        if (!model.meshes)
        {
            throw std::runtime_error("Failed to load model: " + pair.first + " from path: " + pair.second);
        }
        models[pair.first] = model;
    }

    Utilities::Log("Loading shaders...", "Game", LOG_DEBUG);
    for (std::pair<std::string, std::string> pair : gameConfig->shaderPaths)  // Shaders.
    {
        Utilities::Log("Loading: " + pair.first + " from:\n" + pair.second, "Game", LOG_DEBUG);

        std::string vertexShaderPath = pair.second.substr(0, pair.second.find('|'));
        std::string fragmentShaderPath = pair.second.substr(pair.second.find('|') + 1);
        shaders[pair.first] = LoadShader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

        Utilities::Log("Material created for shader: " + pair.first, "Game");
        Material material = LoadMaterialDefault();
        material.shader = shaders[pair.first];
        materials[pair.first] = material;
    }

    // Setting Textures to materials.
    Utilities::Log("Setting textures to materials...", "Game", LOG_DEBUG);
    SetMaterialTexture(&materials["mr-angry-cube-body"], MATERIAL_MAP_DIFFUSE, textures["concrete"]);
    SetMaterialTexture(&materials["mr-angry-cube-face"], MATERIAL_MAP_DIFFUSE, textures["mr-angry-cube-face"]);
    SetMaterialTexture(&materials["static-object"], MATERIAL_MAP_DIFFUSE, textures["texel_checker"]);

    Gui::Init();

    hud = new Hud();

    m_Initialized = true;
}

void Game::UnloadLevel()
{
    delete currentLevel;
    currentLevel = nullptr;
    ResetPlayer();
}

void Game::LoadLevel(std::string levelName)
{
    UnloadLevel();
    Utilities::Log("Loading level: " + levelName + "...", "Game");
    Level* level = new Level(levelName, this);
    if (!level->loaded)
    {
        Utilities::Log("Level not found: " + levelName, "Game", LOG_WARNING);
        return;
    }
    currentLevel = level;
}

MrAngryCube* Game::GetPlayer()
{
    if (m_Player == nullptr)
    {
        Utilities::Log("Player object is not initialized!", "GAME", LOG_ERROR);
        return nullptr;
    }
    return m_Player;
}

void Game::ResetPlayer()
{
    if (m_Player != nullptr) { delete m_Player; }
    m_Player = new MrAngryCube(&models["mr_angry_cube"], {&materials["mr-angry-cube-body"], &materials["mr-angry-cube-face"]});

    if (physicsObserver != nullptr)
    {
        delete physicsObserver;
    }
    
    physicsObserver = new PhysicsObserver(); 
    physicsObserver->observed = m_Player;
    nextRotationAxis = Vector3();
}

void Game::Render()
{
    if (currentLevel == nullptr) { Utilities::Log("Call to render without loading a level!", "Game", LOG_ERROR); return; }

    BeginDrawing();
    ClearBackground(gameConfig->backgroundColor);
        BeginMode3D(*cameraController.camera);
        DrawGrid(200, 1.0f);
        for (GameObject* gameObject : currentLevel->staticObjects)
        {
            gameObject->Render();
        }
        GetPlayer()->Render();
        EndMode3D();
    hud->Render();
    EndDrawing();
}

void Game::HandleGui(Menu* menu)
{
    ClearBackground(gameConfig->backgroundColor);
    BeginDrawing();
    switch (gameState)
    {
        case GameState::MainMenu:
        {
            mainMenu->Update();
            mainMenu->Render();
            if (mainMenu->buttonStates[NEW_GAME_BUTTON_TEXT])
            {
                gameState = GameState::Playing;
                shouldRender = false;
                Utilities::ScheduleWarmUp();
                delete mainMenu;
                mainMenu = nullptr;
            } else if (mainMenu->buttonStates[EXIT_GAME_BUTTON_TEXT])
            {
                shouldRun = false;
                delete mainMenu;
                mainMenu = nullptr;
            } else if (mainMenu->buttonStates[LOAD_LEVEL_BUTTON_TEXT])
            {
                gameState = GameState::LevelSelection;
                delete mainMenu;
                mainMenu = nullptr;
            }
            break;
        }
        case GameState::Paused:
        {
            ClearBackground(gameConfig->backgroundColor);
            if (pauseMenu->buttonStates["Continue"])
            {
                gameState = GameState::Playing;
            } else if (pauseMenu->buttonStates[EXIT_GAME_BUTTON_TEXT])
            {
                shouldRun = false;
            } else if (pauseMenu->buttonStates[RETURN_TO_MAIN_MENU_BUTTON_TEXT])
            {
                gameState = GameState::MainMenu;
                UnloadLevel();
            }
            pauseMenu->Update();
            pauseMenu->Render();
            break;
        }
        case GameState::LevelSelection:
        {
            if (levelMenu->buttonStates[SELECT_LEVEL_BUTTON_TEXT])
            {
                LoadLevel(levelMenu->levels[levelMenu->selected]);
                gameState = GameState::Playing;
            } else if (levelMenu->buttonStates[RETURN_TO_MAIN_MENU_BUTTON_TEXT])
            {
                UnloadLevel();
                gameState = GameState::MainMenu;
            }
            levelMenu->Update();
            levelMenu->Render();
            break;
        }
    }
    EndDrawing();
}

void Game::Update()
{
    m_DeltaTime = GetTime() - m_LastUpdateTime;
    if(m_DeltaTime >= gameConfig->updateTime)
    {
        GetPlayer()->Update(m_DeltaTime);
        cameraController.Update(m_DeltaTime);
        m_LastUpdateTime = GetTime();

        if (physicsObserver!= nullptr)
        {
            physicsObserver->Update();
        }
    }
    if (m_Player->IsAtQuarterRotation(m_Player->rotation))
    {
        currentRotationAxis = nextRotationAxis;
    }
}

void Game::HandleKeyEvents()
{
    if (IsKeyPressed(KEY_ESCAPE)) {
        if (gameState == GameState::Playing)
        {
            gameState = GameState::Paused;
        } else if (gameState == GameState::Paused)
        {
            gameState = GameState::Playing;
        }
    } else if (IsKeyPressed(KEY_ZERO))
    {
        m_Player->SetMoveBehaviour(MoveBehaviourName::NormalMoveBehaviour);
    } else if (IsKeyPressed(KEY_ONE)) {
        m_Player->SetMoveBehaviour(MoveBehaviourName::MoveBehaviourAngerLevel1);
    } else if (IsKeyPressed(KEY_TWO)) {
        m_Player->SetMoveBehaviour(MoveBehaviourName::MoveBehaviourAngerLevel2);
    } else if (IsKeyPressed(KEY_THREE)) {
        m_Player->SetMoveBehaviour(MoveBehaviourName::MoveBehaviourAngerLevel3);
    } else if (IsKeyPressed(KEY_FOUR)) {
        m_Player->SetMoveBehaviour(MoveBehaviourName::MoveBehaviourAngerLevel4);
    } else if (IsKeyPressed(KEY_FIVE)) {
        m_Player->SetMoveBehaviour(MoveBehaviourName::MoveBehaviourAngerLevelInsane);
    } else if (IsKeyPressed(KEY_SIX)) {
        m_Player->SetMoveBehaviour(MoveBehaviourName::NoRotateMoveBehaviour);
    } else if (IsKeyPressed(KEY_W))
    {
        nextRotationAxis = cameraController.GetRightVector();
    } else if (IsKeyPressed(KEY_S))
    {
        nextRotationAxis = cameraController.GetLeftVector();
    } else if (IsKeyPressed(KEY_A))
    {
        nextRotationAxis = cameraController.GetBackVector();
    } else if (IsKeyPressed(KEY_D))
    {
        nextRotationAxis = cameraController.GetFrontVector();
    } else if (IsKeyPressed(KEY_Q))
    {
        nextRotationAxis = { 0, 1, 0 };
    } else if (IsKeyPressed(KEY_E))
    {
        nextRotationAxis = { 0, -1, 0 };
    } else if (IsKeyPressedRepeat(KEY_Z) || IsKeyPressed(KEY_Z))
    {
        cameraController.ZoomIn();
    } else if (IsKeyPressedRepeat(KEY_X) || IsKeyPressed(KEY_X))
    {
        cameraController.ZoomOut();
    } else if (IsKeyPressed(KEY_LEFT))
    {
        cameraController.LeftView();
    } else if (IsKeyPressed(KEY_RIGHT))
    {
        cameraController.RightView();
    } else if (IsKeyPressed(KEY_UP))
    {
        cameraController.MoveUp();
    } else if (IsKeyPressed(KEY_DOWN))
    {
        cameraController.MoveDown();
    } else if (IsKeyPressed(KEY_H))
    {
        hud->visible = !hud->visible;
    }
}

void Game::HandleScheduledEvents()
{
        for (ScheduledEvent* e : timedEvents)
        {
            if (e->triggered) {
                timedEvents.erase(std::remove(timedEvents.begin(), timedEvents.end(), e), timedEvents.end());
                delete e;
                continue;
            }
            e->Update();
        }
}

int Game::Run()
{
    if (!m_Initialized)
    {
        Utilities::Log("Game is not initialized. Exiting...", "Game", LOG_ERROR);
        return 1;
    }
    int returnCode = 0;
    gameState = GameState::MainMenu;
    while (shouldRun)  // Main loop.
    {
        try {
            HandleKeyEvents();
        } catch (const std::exception& e) {
            Utilities::Log("Exception caught in main loop: " + std::string(e.what()), "Game", LOG_ERROR);
            returnCode = 1;
            break;
        } catch (...) {
            Utilities::Log("Unknown exception caught in main loop.", "Game", LOG_ERROR);
            returnCode = 1;
            break;
        }

        try {
            HandleScheduledEvents();
        } catch (const std::exception& e) {
            Utilities::Log("Exception caught during timed events handling: " + std::string(e.what()), "Game", LOG_ERROR);
            returnCode = 1;
            break;
        } catch (...) {
            Utilities::Log("Unknown exception caught during timed events handling.", "Game", LOG_ERROR);
            returnCode = 1;
            break;
        }

        switch (gameState)
        {
            case GameState::MainMenu:
            {
                if (mainMenu == nullptr) { mainMenu = new MainMenu(textures["mr-angry-cube-artwork"]); }
                HandleGui(mainMenu);
                break;
            }
            case GameState::Paused:
            {
                if (pauseMenu == nullptr) { pauseMenu = new PauseMenu(textures["mr-angry-cube-artwork-level-select"]); }
                HandleGui(pauseMenu);
                break;
            }
            case GameState::LevelSelection:
            {
                if (levelMenu == nullptr) { levelMenu = new LevelMenu(textures["mr-angry-cube-artwork-level-select"]); }
                HandleGui(levelMenu);
                break;
            }
            case GameState::Playing:
            {
                if (!currentLevel)
                {
                    LoadLevel("Level-1");
                    if (!currentLevel)
                    {
                        gameState = GameState::MainMenu;
                    } break;
                }

                if (levelMenu) {
                    delete levelMenu;
                    levelMenu = nullptr;

                }
                if (pauseMenu) {
                    delete pauseMenu;
                    pauseMenu = nullptr;
                }
                try {
                    if (shouldUpdate){ Update(); }
                } catch (const std::exception& e) {
                    Utilities::Log("Exception caught during update: " + std::string(e.what()), "Game", LOG_ERROR);
                    returnCode = 1;
                    break;
                } catch (...) {
                    Utilities::Log("Unknown exception caught during update.", "Game", LOG_ERROR);
                    returnCode = 1;
                    break;
                }

                try {
                    if (shouldRender){ Render(); }
                } catch (const std::exception& e) {
                    Utilities::Log("Exception caught during render: " + std::string(e.what()), "Game", LOG_ERROR);
                    returnCode = 1;
                    break;
                } catch (...) {
                    Utilities::Log("Unknown exception caught during render.", "Game", LOG_ERROR);
                    returnCode = 1;
                    break;
                }
            }
        }
        if (WindowShouldClose()){ break; }
    }  // End main loop.
    return returnCode;
}
