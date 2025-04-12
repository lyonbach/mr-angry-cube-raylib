#include "Game.h"


Game::Game()
{
}

Game::Game(std::vector<GameObject*> gameObjects)
{
    for (auto& gameObject : gameObjects)
    {
        Register(gameObject);
    }
}

void Game::Register(GameObject* gameObject)
{
    m_GameObjects.push_back(gameObject);
}

std::vector<Enemy*> Game::GetCollidingEnemies()
{
    std::vector<Enemy*> enemies;
    if (m_GameObjects.empty())
    {
        return enemies;
    }

    MrAngryCube* mrAngryCube = dynamic_cast<MrAngryCube*>(m_GameObjects[0]);
    if (!mrAngryCube)
    {
        return enemies;
    }

    Vector2 mrAngryCubePosition = { mrAngryCube->m_Transform.m12, mrAngryCube->m_Transform.m14 };

    for (auto& gameObject : m_GameObjects)
    {
        if (Enemy* enemy = dynamic_cast<Enemy*>(gameObject))
        {
            Vector2 enemyPosition = { enemy->m_Transform.m12, enemy->m_Transform.m14 };
            if (fabs(mrAngryCubePosition.x - enemyPosition.x) < 0.1f && fabs(mrAngryCubePosition.y - enemyPosition.y) < 0.1f)
            {
                enemies.push_back(enemy);
            }
        }
    }
    return enemies;
}

void Game::Update()
{
    float deltaTime = GetTime() - m_LastUpdateTime;
    if (deltaTime < 1.0f / m_UpdateSpeed)
    {
        return;
    }
    for (auto& gameObject : m_GameObjects)
    {
        gameObject->Update(deltaTime);
    }
    m_LastUpdateTime = GetTime();
}

void Game::Render()
{
    for (auto& gameObject : m_GameObjects)
    {
        gameObject->Render();
    }
}

Game::~Game()
{
    m_GameObjects.clear();
}