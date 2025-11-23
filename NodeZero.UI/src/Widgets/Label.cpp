#include "../../include/Widgets/Label.h"

Label::Label(float x, float y, const char* text, Font font, int fontSize, Color color)
    : m_X(x)
    , m_Y(y)
    , m_Text(text)
    , m_FontSize(fontSize)
    , m_Color(color)
    , m_IsActive(true)
    , m_Font(font)
{
}

void Label::Draw()
{
    if (!m_IsActive)
        return;

    DrawTextEx(m_Font, m_Text.c_str(), Vector2{m_X, m_Y}, static_cast<float>(m_FontSize), 1, m_Color);
}

void Label::Update()
{
}

bool Label::IsHovered() const
{
    return false;
}

bool Label::IsActive() const
{
    return m_IsActive;
}

void Label::SetActive(bool active)
{
    m_IsActive = active;
}

void Label::SetText(const char* text)
{
    m_Text = text;
}

const char* Label::GetText() const
{
    return m_Text.c_str();
}
