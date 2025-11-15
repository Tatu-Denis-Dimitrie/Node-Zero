#pragma once

#include "IWidget.h"
#include <string>

/// <summary>
/// Widget de tip etichetă de text
/// </summary>
class Label : public IWidget
{
private:
    float m_X;
    float m_Y;
    std::string m_Text;
    int m_FontSize;
    Color m_Color;
    bool m_IsActive;

public:
    Label(float x, float y, const char* text, int fontSize = 20, Color color = BLACK);

    void Draw() override;
    void Update() override;
    bool IsHovered() const override;
    bool IsActive() const override;
    void SetActive(bool active) override;

    /// <summary>
    /// Setează textul etichetei
    /// </summary>
    void SetText(const char* text);

    /// <summary>
    /// Obține textul etichetei
    /// </summary>
    const char* GetText() const;
};
