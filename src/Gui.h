#pragma once
#include "raylib.h"
#include "raygui.h"
#include "Utilities.h"


class MainMenu
{
public:
    void RenderAndUpdate();
  
    int buttonWidth = 250;
    int buttonHeight = 50;
    int menuWidth = 500;
    int menuHeight = 640;
    Rectangle rectangle = {
        (float)((GetScreenWidth() - menuWidth) / 2), (float)((GetScreenHeight() - menuHeight)) / 2,
        menuWidth, menuHeight};
        
    Color color = ColorAlpha(RED, .8);
};


namespace Gui
{
    void Init();
}