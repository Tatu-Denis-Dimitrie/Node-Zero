#pragma once

#include "IWidget.h"
#include "raylib.h"
#include <string>
#include <functional>

class Button : public IWidget
{
private:
    float m_X;
    float m_Y;
    float m_Width;
    float m_Height;
    std::string m_Text;
    Color m_NormalColor;
    Color m_HoverColor;
    Color m_PressedColor;
    Color m_TextColor;
    bool m_IsActive;
    std::function<void()> m_OnClick;
    Font m_Font;

public:
    Button(float x, float y, float width, float height, const char* text, Font font);

    void Draw() override;
    void Update() override;
    bool IsHovered() const override;
    bool IsActive() const override;
    void SetActive(bool active) override;

    void SetOnClick(std::function<void()> callback);

    void SetColors(Color normal, Color hover, Color pressed, Color text);
};
