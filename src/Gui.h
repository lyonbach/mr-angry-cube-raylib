#pragma once
#include "raylib.h"
#include "raygui.h"
#include "Utilities.h"
#include <vector>
#include <map>

class MainMenu
{
public:
    int buttonWidth = 250;
    int buttonHeight = 50;
    int menuWidth = 500;
    int menuHeight = 250;
    std::map<std::string, bool> buttonStates;

    MainMenu();
    void RenderAndUpdate();
    std::map<std::string, bool> GetButtonStates();

    Rectangle rectangle = {
        (float)((GetScreenWidth() - menuWidth) / 2), (float)((GetScreenHeight() - menuHeight)) / 2,
    menuWidth, menuHeight};
    Color color = ColorAlpha(RED, .8);
    Color borderColor = ColorAlpha(BLACK, .8);
};


namespace Gui
{
    void Init();
}