#include "Game.h"
#include <algorithm>


void DrawBackgroundTextureFitted(Texture& texture)
{
        Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
        Rectangle destination = { 0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight() };
        DrawTexturePro(texture, source, destination, {0, 0}, 0, WHITE);
}

Game::Game()
{
    Utilities::Log("Created Game singleton class.", "GAME");
}

Game::~Game()
{
    for (auto& gameObject : gameObjects)
    {
        delete gameObject;
    }
    gameObjects.clear();
}

Game& Game::Get() {
    static Game instance;
    return instance;
}

void Game::Init(GameConfig* configuration)
{
    m_GameState = GameState::MainMenu;
    gameConfig = configuration;
    InitWindow(gameConfig->screenWidth, gameConfig->screenHeight, gameConfig->windowTitle.c_str());
    InitMenu();
    SetExitKey(0);  // Disable exit key.
    
    Utilities::Log("Loading textures...", "GAME");
    textures["macDefault"] = LoadTexture(gameConfig->texturePaths["macDefault"].c_str());
    textures["mainMenuBackground"] = LoadTexture(gameConfig->texturePaths["mainMenuBackground"].c_str());
    textures["enemyDefault"] = textures["macDefault"];
    Utilities::Log("Textures loaded.", "GAME");

    Utilities::Log("Loading shaders...", "GAME");
    shaders["macDefault"] = LoadShader(0, gameConfig->shaderPaths["macDefault"].c_str());
    shaders["enemyDefault"] = shaders["macDefault"];
    Utilities::Log("Shaders loaded.", "GAME");

    Utilities::Log("Loading models...", "GAME");
    models["macDefault"] = LoadModel(gameConfig->modelPaths["macDefault"].c_str());
    // models["enemyDefault"];
    Utilities::Log("Models loaded.", "GAME");
    
    // Initialize main character.
    mrAngryCube = new MrAngryCube(models["macDefault"], shaders["macDefault"], textures["macDefault"]);
    Register(mrAngryCube);

    LoadLevel("TestLevel");
    m_Initialized = true;
}

void Game::InitMenu()
{
    m_Menu = new Menu();
    m_Menu->AddItem(
        new PushButton("     Play     ",
            gameConfig->screenWidth / 2,
            gameConfig->screenHeight / 3,
            [this](){ m_GameState = GameState::Playing; })
    );
    m_Menu->AddItem(
        new PushButton("     Exit     ",
            gameConfig->screenWidth / 2,
            gameConfig->screenHeight / 3 + 50,
            [](){ exit(0); })
    );
}

void Game::SpawnEnemy()
{
    m_GameMode.spawnBehaviour();
}

void Game::LoadLevel(const char* levelName)
{
    Utilities::Log("Loading:" + std::string(levelName), "GAME");
}

void Game::Register(GameObject* gameObject)
{
    gameObjects.push_back(gameObject);
}

void Game::Unregister(GameObject* gameObject)
{
    auto it = std::remove(gameObjects.begin(), gameObjects.end(), gameObject);
    if (it != gameObjects.end())
    {
        gameObjects.erase(it);
    }
}

std::vector<Enemy*> Game::GetCollidingEnemies()
{
    std::vector<Enemy*> enemies;
    if (gameObjects.empty())
    {
        return enemies;
    }

    if (!mrAngryCube)
    {
        return enemies;
    }

    Vector2 mrAngryCubePosition = { mrAngryCube->transform.m12, mrAngryCube->transform.m14 };

    for (auto& gameObject : gameObjects)
    {
        if (Enemy* enemy = dynamic_cast<Enemy*>(gameObject))
        {
            Vector2 enemyPosition = { enemy->transform.m12, enemy->transform.m14 };
            if (fabs(mrAngryCubePosition.x - enemyPosition.x) < 0.1f && fabs(mrAngryCubePosition.y - enemyPosition.y) < 0.1f)
            {
                enemies.push_back(enemy);
            }
        }
    }
    return enemies;
}

std::vector<Enemy*> Game::GetEnemies()
{
    std::vector<Enemy*> enemies;
    if (gameObjects.empty())
    {
        return enemies;
    }
    if (!mrAngryCube)
    {
        return enemies;
    }

    for (auto& gameObject : gameObjects)
    {
        if (Enemy* enemy = dynamic_cast<Enemy*>(gameObject))
        {
            enemies.push_back(enemy);
        }
    }
    return enemies;
}

void Game::Update(float deltaTime)
{
    if (m_GameState == GameState::Playing)
    {
        m_CamController.Update(deltaTime, mrAngryCube);
        for (auto& gameObject : gameObjects)
        {
            gameObject->Update(deltaTime);
        }
        for (Enemy* enemy : GetCollidingEnemies())
        {
            Unregister(enemy);
            gameInfo.score++;
            gameInfo.anger = std::max(0, --gameInfo.anger);
            gameInfo.angerIncrementCountdown = gameInfo.defaultAngerIncrementCountdown;
            gameInfo.gameOverCountdown = gameInfo.defaultGameOverCountDown;

            timedTexts.clear();
            const char* quote = Utilities::GetQuote(Reason::Smash);
            timedTexts.push_back(Utilities::GetTimedText(quote, Reason::Smash));
        }
        if (gameInfo.gameOverCountdown <= 0)
        {
            m_GameState = GameState::GameOver;
        }
    }
}

void Game::Render()
{
    if (!m_Initialized)
    {
        TraceLog(LOG_WARNING, "Game not initialized!");
        return;
    }

    BeginDrawing();
    ClearBackground(gameConfig->backgroundColor);
    switch (m_GameState)
    {
        case GameState::Playing:
            m_CamController.Render();
        break;
    }
    RenderHud();
    EndDrawing();
}

void Game::RenderHud()
{
    std::vector<std::string> textsToRender;
    switch (m_GameState)
    {
        case GameState::MainMenu:
        {
            ClearBackground(gameConfig->backgroundColor);
            DrawBackgroundTextureFitted(textures["mainMenuBackground"]);
            m_Menu->Update();
            m_Menu->Render();
        }
        break;
        case GameState::Playing:
        {
            for(auto it=timedTexts.begin(); it!=timedTexts.end();)
            {
                auto timedText = *it;
                if(GetTime() - timedText->lastCheckTime > timedText->duration)
                {
                    it = timedTexts.erase(it);
                } else {
                    timedText->Draw();
                    it++;
                }
            }

            // Draw score. FIXME SHOULD BE IMPROVED
            int fontSize = 20;
            int percentage = (int)((float)gameInfo.anger / (gameInfo.possibleSpeeds.size() - 1) * 100);
            int rotations = gameInfo.rotationCount.x + gameInfo.rotationCount.y + gameInfo.rotationCount.z;
            textsToRender = {
                std::string("Score: " + std::to_string(gameInfo.score)),
                std::string("Anger: " + std::to_string(percentage) + "%"),
                std::string("Enemies Alive: " + std::to_string(GetEnemies().size())),
                std::string("Rotations: " + std::to_string(rotations))
            };
            for (size_t i = 0; i < textsToRender.size(); i++)
            {
                std::string text = textsToRender.at(i);
                DrawText(text.c_str(), 2*fontSize, (2 + 2 * i) * fontSize, fontSize, YELLOW);
            }
        }
        break;
        case GameState::Paused:
        {
            int fontSize = 30;
            const char* text = "GAME PAUSED";
            DrawText(text, (GetScreenWidth() - MeasureText(text, fontSize)) / 2, (GetScreenHeight() - fontSize) / 2, fontSize, WHITE);
        }
        break;
        case GameState::GameOver:
        {
            int fontSize = 30;
            const char* text = "GAME OVER";
            DrawText(text, (GetScreenWidth() - MeasureText(text, fontSize)) / 2, (GetScreenHeight() - fontSize) / 2, fontSize, WHITE);
        }
        break;
    }
    DrawFPS(GetScreenWidth() - 200, 50);
}

int Game::Run()
{
    float deltaTime = 5.0f;
    while (!WindowShouldClose())
    {
        HandleKeyEvents();
        deltaTime = GetTime() - gameInfo.lastUpdateTime;
        if(GetTime() - gameInfo.lastUpdateTime > 1.0f / gameConfig->updateSpeed)
        {
            Update(deltaTime);
            gameInfo.lastUpdateTime = GetTime();
        }
        Render();
    }
    return 0;
}

void Game::HandleKeyEvents()
{
    if (IsKeyPressed(KEY_W))
    {
        mrAngryCube->nextRotationAxis = m_CamController.GetRightVector();
    } else if (IsKeyPressed(KEY_S))
    {
        mrAngryCube->nextRotationAxis = m_CamController.GetLeftVector();
    } else if (IsKeyPressed(KEY_A))
    {
        mrAngryCube->nextRotationAxis = m_CamController.GetRearVector();
    } else if (IsKeyPressed(KEY_D))
    {
        mrAngryCube->nextRotationAxis = m_CamController.GetFrontVector();
    } else if (IsKeyPressed(KEY_E))
    {
        mrAngryCube->nextRotationAxis = { 0.0f, -1.0f, 0.0f };
    } else if (IsKeyPressed(KEY_Q))
    {
        mrAngryCube->nextRotationAxis = { 0.0f, 1.0f, 0.0f };
    } else if (IsKeyPressed(KEY_R))
    {
        SpawnEnemy();
    } else if (IsKeyPressed(KEY_ESCAPE)) {

        switch (m_GameState)
        {
            case GameState::Playing:
                m_GameState = GameState::Paused;
            break;

            case GameState::Paused:
                m_GameState = GameState::Playing;
            break;

            case GameState::GameOver:
                m_GameState = GameState::MainMenu;

            default:
            break;
        }
    } else if (IsKeyPressed(KEY_RIGHT)) {
        m_CamController.RotateCamera(RotationDirection::CW);
    } else if (IsKeyPressed(KEY_LEFT)) {
        m_CamController.RotateCamera(RotationDirection::CCW);
    } else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_UP)) {
        m_CamController.RotateCamera(RotationDirection::CCW);
        m_CamController.RotateCamera(RotationDirection::CCW);
    }
}

void Game::Exit()
{
    CloseWindow();
    exit(0);
}
