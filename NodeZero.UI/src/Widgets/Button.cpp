#include "Widgets/Button.h"

Button::Button(float x, float y, float width, float height, const char* text, Font font)
    : m_X(x), m_Y(y), m_Width(width), m_Height(height), m_Text(text), m_NormalColor(LIGHTGRAY), m_HoverColor(GRAY), m_PressedColor(DARKGRAY), m_TextColor(BLACK), m_IsActive(true), m_OnClick(nullptr), m_Font(font) {
}

void Button::Draw() {
    if (!m_IsActive)
        return;

    Color currentColor = m_NormalColor;

    if (IsHovered()) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            currentColor = m_PressedColor;
        else
            currentColor = m_HoverColor;
    }

    DrawRectangle(
        static_cast<int>(m_X),
        static_cast<int>(m_Y),
        static_cast<int>(m_Width),
        static_cast<int>(m_Height),
        currentColor);

    DrawRectangleLines(
        static_cast<int>(m_X),
        static_cast<int>(m_Y),
        static_cast<int>(m_Width),
        static_cast<int>(m_Height),
        BLACK);

    Vector2 textSize = MeasureTextEx(m_Font, m_Text.c_str(), 20, 1);
    float textX = m_X + (m_Width - textSize.x) / 2;
    float textY = m_Y + (m_Height - textSize.y) / 2;

    DrawTextEx(m_Font, m_Text.c_str(), Vector2{textX, textY}, 20, 1, m_TextColor);
}

void Button::Update() {
    if (!m_IsActive)
        return;

    if (IsHovered() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (m_OnClick)
            m_OnClick();
    }
}

bool Button::IsHovered() const {
    Vector2 mousePos = GetMousePosition();
    return mousePos.x >= m_X && mousePos.x <= m_X + m_Width &&
           mousePos.y >= m_Y && mousePos.y <= m_Y + m_Height;
}

bool Button::IsActive() const {
    return m_IsActive;
}

void Button::SetActive(bool active) {
    m_IsActive = active;
}

void Button::SetOnClick(std::function<void()> callback) {
    m_OnClick = callback;
}

void Button::SetColors(Color normal, Color hover, Color pressed, Color text) {
    m_NormalColor = normal;
    m_HoverColor = hover;
    m_PressedColor = pressed;
    m_TextColor = text;
}
