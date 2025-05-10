#pragma once
#include "raylib.h"
#include "raygui.h"
#include "Utilities.h"
#include <filesystem>
#include <map>
#include <vector>


class Menu
{
public:
    float buttonOffsetY = 20.0f;
    float buttonWidth = 250.0f;
    float buttonHeight = 50.0f;
    float menuWidth = 500.0f;
    float menuHeight = 250.0f;
    float menuX = static_cast<float>(GetScreenWidth() / 8);
    float menuY = static_cast<float>((GetScreenHeight() - menuHeight) / 2);

    std::map<std::string, bool> buttonStates;
    Color color = ColorAlpha(RED, .8);
    Color borderColor = ColorAlpha(BLACK, .8);
    Rectangle rectangle = { menuX, menuY, menuWidth, menuHeight };
    std::map<std::string, bool> GetButtonStates();

    Menu(Texture& background);
    void Render();
    void Update();

private:
    Texture& m_BackgroundTexture;

};


class MainMenu : public Menu
{
public:
    MainMenu(Texture& background);
};


class PauseMenu : public Menu
{
public:
    PauseMenu(Texture& background);
};


class LevelMenu : public Menu
{
public:
    float comboBoxY = GetScreenHeight() - 20.0 - (rectangle.y + (buttonHeight + buttonOffsetY) * 2);
    std::vector<std::string> levels;
    std::string levelsText;
    int selected = 0;
    int clicked = 0;

    LevelMenu(Texture& background);
    void Render();
};


namespace Gui
{
    void Init();
}