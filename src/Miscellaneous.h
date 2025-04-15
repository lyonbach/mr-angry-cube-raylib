#pragma once

enum class GameState {
    MainMenu,
    Playing,
    Paused,
    GameOver
};

struct GameConfig {
    GameConfig(
        int targetFPS,
        int updateSpeed,
        int screenWidth,
        int screenHeight,
        const char* windowTitle,
        const char* texturePath,
        const char* shaderPath,
        const char* modelPath)
        : m_TargetFPS(targetFPS),
        m_UpdateSpeed(updateSpeed),
        m_ScreenWidth(screenWidth),
        m_ScreenHeight(screenHeight),
        m_TexturePath(texturePath),
        m_ShaderPath(shaderPath),
        m_ModelPath(modelPath),
        m_WindowTitle(windowTitle) {}
    int m_TargetFPS;
    int m_UpdateSpeed;
    int m_ScreenWidth;
    int m_ScreenHeight;
    const char* m_TexturePath;
    const char* m_ShaderPath;
    const char* m_ModelPath;
    const char* m_WindowTitle;
};

struct GameInfo {
    int score = 0;
    float anger = 1.0f;
    float maxAnger = 10.0f;
};