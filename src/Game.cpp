#include "Game.h"
#include <algorithm>


Game& Game::Get() {
    static Game instance;
    return instance;
}

Game::Game()
{
    Utilities::Log("Created Game singleton class.", "GAME");
}

void Game::Init(GameConfig* configuration)
{
    m_GameState = GameState::Playing;
    m_GameConfig = configuration;
    InitWindow(m_GameConfig->screenWidth, m_GameConfig->screenHeight, m_GameConfig->windowTitle.c_str());
    InitMenu();
    SetExitKey(0);  // Disable exit key.

    // Initialize main character.
    m_MrAngryCube = new MrAngryCube(
        m_GameConfig->texturePath.c_str(),
        m_GameConfig->shaderPath.c_str(),
        m_GameConfig->modelPath.c_str()
    );
    Register(m_MrAngryCube);
    m_Initialized = true;
}

Game::~Game()
{
    for (auto& gameObject : gameObjects)
    {
        delete gameObject;
    }
    gameObjects.clear();
}

void Game::InitMenu()
{
    m_Menu = new Menu();
    m_Menu->AddItem(
        new PushButton("     Play     ",
            m_GameConfig->screenWidth / 2,
            m_GameConfig->screenHeight / 3,
            [this](){ m_GameState = GameState::Playing; })
    );
    m_Menu->AddItem(
        new PushButton("     Exit     ",
            m_GameConfig->screenWidth / 2,
            m_GameConfig->screenHeight / 3 + 50,
            [](){ exit(0); })
    );
}

void Game::SpawnEnemy(Vector2 coordinates)
{
    // Enemies can appear next to the Mr. Angry Cube or 20 unit distance from him.
    // In addition, each coordinate should be divisible to 2.
    int randX = GetRandomValue(2, 20);
    int randZ = GetRandomValue(2, 20);
    if (randX % 2 != 0) randX++;
    if (randZ % 2 != 0) randZ++;

    Enemy* enemy = new Enemy(
        m_GameConfig->texturePath.c_str(),
        m_GameConfig->shaderPath.c_str(),
        m_GameConfig->modelPath.c_str()
    );
    enemy->SetPosition({(float)randX, .5, (float)randZ}); // FIXME GET ENEMYSIZE
    Register(enemy);
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

    if (!m_MrAngryCube)
    {
        return enemies;
    }

    Vector2 mrAngryCubePosition = { m_MrAngryCube->transform.m12, m_MrAngryCube->transform.m14 };

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
    if (!m_MrAngryCube)
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

void Game::Update()
{
    if (m_GameState != GameState::Playing) { return; }
    // m_CamController.Update(m_MrAngryCube); // Update camera.

    float deltaTime = GetTime() - m_LastUpdateTime;
    if (deltaTime < 1.0f / m_GameConfig->updateSpeed)
    {
        return;
    }

    for (auto& gameObject : gameObjects)
    {
        gameObject->Update(deltaTime);
    }

    if(m_MrAngryCube->IsAtQuarterRotation() && m_MrAngryCube->isMoving)
    {
        bool shouldIncreaseAnger = false;
        const char* quote = "";

        int rotationCountSum = Utilities::SumVector3(m_MrAngryCube->rotationCount);  // FIXME BETTER NAMES

        if (Game::Get().gameInfo.anger >= Game::Get().gameInfo.possibleSpeeds.size() - 1)
        {
            Game::Get().gameInfo.rotationCountdown += -1;
            TraceLog(LOG_WARNING, "Game over in: %i rotations.", Game::Get().gameInfo.rotationCountdown);  // FIXME SHOW TO USER BETTER.
        } else if (Game::Get().gameInfo.anger < Game::Get().gameInfo.possibleSpeeds.size() - 1)
        {
            GameInfo tempGameInfo;
            Game::Get().gameInfo.rotationCountdown = tempGameInfo.rotationCountdown;
        }

        if (rotationCountSum % 10 == 0 && rotationCountSum > 0)
        {
            quote = "Dizzy and angry!!!";  // FIXME GET RANDOM QUOTE FROM A BUNCH OF QUOTES.
            shouldIncreaseAnger = true;
        }

        if (m_MrAngryCube->IsFaceOnTheGround())
        {
            quote = "My Face!\n No more Mr. Nice Cube!";  // FIXME GET RANDOM QUOTE FROM A BUNCH OF QUOTES.
            shouldIncreaseAnger = true;

            // Create a text to be displayed. FIXME THIS LOGIC WILL BE CHANGED TO SHOW SOME TEXTURE OR DECAL.
            m_MrAngryCube->WaitForNonBlocking(.2f * 3);  // FIXME MOVE TO GAME CONFIG
        } else {
            m_MrAngryCube->WaitForNonBlocking(.2f);  // FIXME MOVE TO GAME CONFIG
        }

        // Handle anger.
        if (shouldIncreaseAnger)
        {
            gameInfo.anger = std::min((int)gameInfo.possibleSpeeds.size() - 1, ++gameInfo.anger);
            m_MrAngryCube->speed = gameInfo.possibleSpeeds.at(gameInfo.anger);
        }

        // Handle quotes.
        if (quote != "")
        {
            TimedText* timedText = Utilities::GetTimedText(quote);
            timedText->lastCheckTime = GetTime();
            timedTexts.push_back(timedText);
        }
    }

    // Check collisions.
    for (Enemy* enemy : GetCollidingEnemies())
    {
        if (m_MrAngryCube->IsFaceOnTheGround())
        {
            break;
        }
        Unregister(enemy);
        gameInfo.score++;
        gameInfo.anger = std::max(0, --gameInfo.anger);
    }

    if (gameInfo.rotationCountdown == 0)
    {
        m_GameState = GameState::GameOver;
    }

    m_LastUpdateTime = GetTime();
}

void Game::Render()
{
    if (!m_Initialized)
    {
        TraceLog(LOG_WARNING, "Game not initialized!");
        return;
    }

    BeginDrawing();
    ClearBackground(m_GameConfig->backgroundColor);
    switch (m_GameState)
    {
        case GameState::MainMenu:
        m_Menu->Update();
        m_Menu->Render();
        break;
        case GameState::Playing:
            m_CamController.Render();
        break;
        case GameState::Paused:
        {
            // Do nothing.
        }
        case GameState::GameOver:
            ClearBackground(DARKGREEN);
        break;
        default:
            TraceLog(LOG_ERROR, "Unknown game state!");
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
        case GameState::Playing:
        {
            // FIXME THIS IS NOT A GOOD WAY TO DO IT. THINK ABOUT THE EVENT SYSTEM.
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
            int rotations = m_MrAngryCube->rotationCount.x + m_MrAngryCube->rotationCount.y + m_MrAngryCube->rotationCount.z;
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
}

int Game::Run()
{

    m_CamController.Run(m_MrAngryCube);
    while (!WindowShouldClose())  // Main loop.
    {
        // Handle key events.
        //----------------------------------------------------------------------------------
        if (IsKeyPressed(KEY_W))
        {
            m_MrAngryCube->nextRotationAxis = { 1.0f, 0.0f, 0.0f };
        } else if (IsKeyPressed(KEY_S))
        {
            m_MrAngryCube->nextRotationAxis = { -1.0f, 0.0f, 0.0f };
        } else if (IsKeyPressed(KEY_A))
        {
            m_MrAngryCube->nextRotationAxis = { 0.0f, 0.0f, -1.0f };
        } else if (IsKeyPressed(KEY_D))
        {
            m_MrAngryCube->nextRotationAxis = { 0.0f, 0.0f, 1.0f };
        } else if (IsKeyPressed(KEY_E))
        {
            m_MrAngryCube->nextRotationAxis = { 0.0f, -1.0f, 0.0f };
        } else if (IsKeyPressed(KEY_Q))
        {
            m_MrAngryCube->nextRotationAxis = { 0.0f, 1.0f, 0.0f };
        } else if (IsKeyPressed(KEY_R))
        {
            SpawnEnemy({0, 0});
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
       } else if (IsKeyPressed(KEY_F)) {
            ToggleFullscreen();
       }
        //----------------------------------------------------------------------------------
        Update();
        Render();
    }
    return 0;
}

void Game::Exit()
{
    CloseWindow();
    exit(0);
}
