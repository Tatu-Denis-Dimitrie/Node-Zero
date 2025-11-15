#include "../../include/Widgets/Slider.h"
#include <algorithm>

Slider::Slider(float x, float y, float width, float height, float minValue, float maxValue, float initialValue)
    : m_X(x)
    , m_Y(y)
    , m_Width(width)
    , m_Height(height)
    , m_MinValue(minValue)
    , m_MaxValue(maxValue)
    , m_CurrentValue(initialValue)
    , m_BarColor(LIGHTGRAY)
    , m_HandleColor(DARKGRAY)
    , m_IsActive(true)
    , m_IsDragging(false)
{
}

void Slider::Draw()
{
    if (!m_IsActive)
        return;

    DrawRectangle(
        static_cast<int>(m_X),
        static_cast<int>(m_Y),
        static_cast<int>(m_Width),
        static_cast<int>(m_Height),
        m_BarColor
    );

    DrawRectangleLines(
        static_cast<int>(m_X),
        static_cast<int>(m_Y),
        static_cast<int>(m_Width),
        static_cast<int>(m_Height),
        BLACK
    );

    float normalizedValue = (m_CurrentValue - m_MinValue) / (m_MaxValue - m_MinValue);
    float handleX = m_X + normalizedValue * m_Width;

    DrawCircle(
        static_cast<int>(handleX),
        static_cast<int>(m_Y + m_Height / 2),
        m_Height,
        m_HandleColor
    );

    DrawCircleLines(
        static_cast<int>(handleX),
        static_cast<int>(m_Y + m_Height / 2),
        m_Height,
        BLACK
    );
}

void Slider::Update()
{
    if (!m_IsActive)
        return;

    Vector2 mousePos = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && IsHovered())
    {
        m_IsDragging = true;
    }

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        m_IsDragging = false;
    }

    if (m_IsDragging)
    {
        float normalizedX = (mousePos.x - m_X) / m_Width;
        normalizedX = std::max(0.0f, std::min(1.0f, normalizedX));
        m_CurrentValue = m_MinValue + normalizedX * (m_MaxValue - m_MinValue);
    }
}

bool Slider::IsHovered() const
{
    Vector2 mousePos = GetMousePosition();
    return mousePos.x >= m_X && mousePos.x <= m_X + m_Width &&
           mousePos.y >= m_Y - m_Height && mousePos.y <= m_Y + m_Height * 2;
}

bool Slider::IsActive() const
{
    return m_IsActive;
}

void Slider::SetActive(bool active)
{
    m_IsActive = active;
}

float Slider::GetValue() const
{
    return m_CurrentValue;
}

void Slider::SetValue(float value)
{
    m_CurrentValue = std::max(m_MinValue, std::min(m_MaxValue, value));
}
