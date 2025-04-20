#pragma once

#include "raylib.h"
#include <string>
#include <functional>

typedef std::function<void()> ButtonCallback;

class GuiItem
{
public:
    Vector2 m_Position;

    virtual void Render()   = 0;
    virtual void Update()   = 0;
    virtual int GetWidth()  = 0;
    virtual int GetHeight() = 0;
};

class PushButton : public GuiItem {
    public:
        std::string m_Text;
        Vector2 m_Position;

        PushButton(const std::string& text, int x, int y, ButtonCallback callbackFunction=nullptr);
        void Render() override;
        void Update() override;
        int GetWidth() override;
        int GetHeight() override;
        void DoAction();

        private:
        ButtonCallback callback;
        int fontSize;
        int paddingX;
        int paddingY;
        int textWidth;
        int startX;
        int startY;
        int endX;
        int endY;
        bool shouldUpdate;
        Color currentBackgroundColor;
};

class Menu
{
public:
    Menu();
    ~Menu();
    void AddItem(GuiItem* item);
    void Render();
    void Update();

    std::vector<GuiItem*> m_Items;
};