#include "../../include/Widgets/Label.h"

Label::Label(float x, float y, const char* text, int fontSize, Color color)
    : m_X(x)
    , m_Y(y)
    , m_Text(text)
    , m_FontSize(fontSize)
    , m_Color(color)
    , m_IsActive(true)
{
}

void Label::Draw()
{
    if (!m_IsActive)
        return;

    DrawText(m_Text.c_str(), static_cast<int>(m_X), static_cast<int>(m_Y), m_FontSize, m_Color);
}

void Label::Update()
{
    // Labels nu necesită actualizare
}

bool Label::IsHovered() const
{
    return false; // Labels nu sunt interactive
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
