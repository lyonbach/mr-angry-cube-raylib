#include "MrAngryCube.h"
#include "Game.h"
#include "Constants.h"
#include "Gui.h"
#include "StaticObject.h"
#include <stdexcept>


Game::Game()
{
    Utilities::Log("Created Game singleton class.", "Game");
}

Game::~Game()
{
    delete physicsObserver;
    gameObjects.clear();
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

    // Initialize models, shaders, textures and materials.
    Utilities::Log("Loading models...", "Game");  // Models.
    for (std::pair<std::string, std::string> pair : gameConfig->modelPaths)
    {
        Utilities::Log("Loading: " + pair.first + " from:\n" + pair.second, "GAME");
        models[pair.first] = LoadModel(pair.second.c_str());
    }

    Utilities::Log("Loading shaders...", "Game");
    for (std::pair<std::string, std::string> pair : gameConfig->shaderPaths)  // Shaders.
    {
        Utilities::Log("Loading: " + pair.first + " from:\n" + pair.second, "GAME");

        std::string vertexShaderPath = pair.second.substr(0, pair.second.find('|'));
        std::string fragmentShaderPath = pair.second.substr(pair.second.find('|') + 1);
        shaders[pair.first] = LoadShader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

        Utilities::Log("Material created for shader: " + pair.first, "GAME");
        Material material = LoadMaterialDefault();
        material.shader = shaders[pair.first];
        materials[pair.first] = material;
    }

    Utilities::Log("Loading textures...", "GAME");
    for (std::pair<std::string, std::string> pair : gameConfig->texturePaths) // Textures.
    {
        Utilities::Log("Loading: " + pair.first + " from:\n" + pair.second, "GAME");
        textures[pair.first] = LoadTexture(pair.second.c_str());
    }

    // Initialize game objects.
    MrAngryCube* player = new MrAngryCube(&models["macDefault"], &materials["macDefault"], &textures["macDefault"]);
    m_Player = player;
    Register(player);

    physicsObserver = new PhysicsObserver();
    physicsObserver->observed = player;

    Gui::Init();

    m_Initialized = true;
}

void Game::Register(GameObject* newGameObject)
{
    for (GameObject* gameObject : gameObjects)
    {
        if (gameObject->objectId == newGameObject->objectId)
        {
            Utilities::Log("Can not register object with id \"" + newGameObject->objectId + "\"!\nAlready registered.", "GAME", LOG_ERROR);
            return;
        }
    }
    gameObjects.push_back(newGameObject);
}

void Game::LoadLevel(std::string filePath)
{
    Utilities::Log("Loading level from " + filePath + "...", "Game");
    Level* level = new Level(filePath, this);
    if (!level->loaded)
    {
        Utilities::Log("Level not found: " + filePath, "Game", LOG_WARNING);
        return;
    }
    currentLevel = level;
    for (auto obj : level->staticObjects)
    {
        // Register(obj);
    }
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

void Game::Render()
{
    BeginDrawing();
    ClearBackground(gameConfig->backgroundColor);
        BeginMode3D(*cameraController.camera);
        DrawGrid(200, 1.0f);
        for (GameObject* gameObject : gameObjects)
        {
            gameObject->Render();
        }
        EndMode3D();
    EndDrawing();
}

void Game::Update()
{
    m_DeltaTime = GetTime() - m_LastUpdateTime;
    if(m_DeltaTime >= gameConfig->updateTime)
    {
        for (GameObject* gameObject : gameObjects)
        {
            gameObject->Update(m_DeltaTime);
        }

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
    }
}

int Game::Run()
{

    if (!m_Initialized)
    {
        Utilities::Log("Game is not initialized. Exiting...", "GAME", LOG_ERROR);
        return 1;
    }

    int returnCode = 0;
    gameState = GameState::MainMenu;
    while (m_ShouldRun)  // Main loop.
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

        switch (gameState)
        {
            case GameState::MainMenu:
            {
                if (mainMenu == nullptr) { mainMenu = new MainMenu(); }
                if (mainMenu->buttonStates[NEW_GAME_BUTTON_TEXT])
                {
                    gameState = GameState::Playing;
                } else if (mainMenu->buttonStates[EXIT_GAME_BUTTON_TEXT])
                {
                    m_ShouldRun = false;
                } else if (mainMenu->buttonStates[LOAD_LEVEL_BUTTON_TEXT])
                {
                    gameState = GameState::LevelSelection;
                }
                mainMenu->Update();

                BeginDrawing();
                    Texture* texture = &textures["mainMenuBackground"];
                    float offsetX = (GetScreenWidth() - texture->width) / 2;
                    float offsetY = (GetScreenHeight() - texture->height) / 2;
                    DrawTextureEx(textures["mainMenuBackground"], {offsetX, offsetY}, 0, 1, WHITE);
                    mainMenu->Render();
                EndDrawing();
                break;
            }
            case GameState::Paused:
            {
                if (pauseMenu == nullptr) { pauseMenu = new PauseMenu(); }
                if (pauseMenu->buttonStates["Continue"])
                {
                    gameState = GameState::Playing;
                } else if (pauseMenu->buttonStates[EXIT_GAME_BUTTON_TEXT])
                {
                    m_ShouldRun = false;
                } else if (pauseMenu->buttonStates[RETURN_TO_MAIN_MENU_TEXT])
                {
                    gameState = GameState::MainMenu;
                }
                pauseMenu->Update();
                BeginDrawing();
                pauseMenu->Render();
                EndDrawing();
                break;
            }
            case GameState::LevelSelection:
            {
                BeginDrawing();
                Texture* texture = &textures["levelSelectionMenuBackground"];
                float offsetX = (GetScreenWidth() - texture->width) / 2;
                float offsetY = (GetScreenHeight() - texture->height) / 2;
                DrawTextureEx(textures["levelSelectionMenuBackground"], {offsetX, offsetY}, 0, 1, WHITE);

                EndDrawing();
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

                delete pauseMenu;
                delete mainMenu;
                pauseMenu = nullptr;
                mainMenu = nullptr;

                try {
                    Update();
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
                    Render();
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

    }

    return returnCode;
}
