#define RAYGUI_IMPLEMENTATION
#include "Gui.h"

void MainMenu::RenderAndUpdate()
{
    DrawRectangle(rectangle.x, rectangle.y, rectangle.width, rectangle.height, color);
    GuiButton(
        {rectangle.x + (rectangle.width / 2 - buttonWidth / 2), rectangle.y + 30, (float)buttonWidth, (float)buttonHeight},
        "New Game"
    );
}


// Gui::Init()
// {
//     GuiLoadStyle;
// }