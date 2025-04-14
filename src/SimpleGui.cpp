#include "SimpleGui.h"


PushButton::PushButton(const std::string& text, int x, int y, ButtonCallback callbackFunction)
    : m_Text(text),
      m_Position({ (float)x, (float)y })
{
    fontSize = 20;
    paddingX = 10;
    paddingY = 10;
    textWidth = MeasureText(m_Text.c_str(), fontSize);
    startX = (int)m_Position.x - paddingX - textWidth / 2;
    startY = (int)m_Position.y - paddingY;
    endX = startX + textWidth + paddingX * 2;
    endY = startY + paddingY * 2 + fontSize;
    currentBackgroundColor = DARKGRAY;
    shouldUpdate = true;
    callback=callbackFunction;
}

void PushButton::Render()
{
    DrawRectangle(startX, startY, endX - startX, endY - startY, currentBackgroundColor);
    DrawText(m_Text.c_str(), startX + paddingX, startY + paddingY, fontSize, WHITE);
}

void PushButton::Update()
{
    if(!shouldUpdate) return;

    Vector2 mousePos = GetMousePosition();
    if (mousePos.x >= startX && mousePos.x <= endX && mousePos.y >= startY && mousePos.y <= endY) {
        currentBackgroundColor = ORANGE;
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            currentBackgroundColor = RED;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            DoAction();
        }
    } else {
        currentBackgroundColor = DARKGRAY;
    }
}

void PushButton::DoAction()
{
    if (callback != nullptr)
    {
        callback();
        shouldUpdate = false;
    }
}

int PushButton::GetWidth()
{
    return endX - startX;
}

int PushButton::GetHeight()
{
    return endY - startY;
}

Menu::Menu() {}

void Menu::AddItem(GuiItem* item)
{
    m_Items.push_back(item);
}

void Menu::Render()
{
    for (GuiItem* item : m_Items)
    {
        item->Render();
    }
}

void Menu::Update()
{
    for (GuiItem* item : m_Items)
    {
        item->Update();
    }
}

Menu::~Menu()
{
    for (GuiItem* item : m_Items)
    {
        delete item;
    }
}