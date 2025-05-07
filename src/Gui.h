#pragma once
#include "raylib.h"
#include "raygui.h"
#include "Utilities.h"
#include <vector>
#include <map>


class Menu
{
public:
    int buttonWidth = 250;
    int buttonHeight = 50;
    int menuWidth = 500;
    int menuHeight = 250;
    std::map<std::string, bool> buttonStates;
    Color color = ColorAlpha(RED, .8);
    Color borderColor = ColorAlpha(BLACK, .8);
    Rectangle rectangle = {
        (float)((GetScreenWidth() - menuWidth) / 2), (float)((GetScreenHeight() - menuHeight)) / 2,
    static_cast<float>(menuWidth), static_cast<float>(menuHeight)};

    std::map<std::string, bool> GetButtonStates();
    void Render();
    void Update();

};


class MainMenu : public Menu
{
public:
    MainMenu();
};


class PauseMenu : public Menu
{
public:
    PauseMenu();
};


namespace Gui
{
    void Init();
}