#pragma once

#include "IWidget.h"

/// <summary>
/// Widget de tip slider (pentru setări)
/// </summary>
class Slider : public IWidget
{
private:
    float m_X;
    float m_Y;
    float m_Width;
    float m_Height;
    float m_MinValue;
    float m_MaxValue;
    float m_CurrentValue;
    Color m_BarColor;
    Color m_HandleColor;
    bool m_IsActive;
    bool m_IsDragging;

public:
    Slider(float x, float y, float width, float height, float minValue, float maxValue, float initialValue);

    void Draw() override;
    void Update() override;
    bool IsHovered() const override;
    bool IsActive() const override;
    void SetActive(bool active) override;

    /// <summary>
    /// Obține valoarea curentă a slider-ului
    /// </summary>
    float GetValue() const;

    /// <summary>
    /// Setează valoarea curentă a slider-ului
    /// </summary>
    void SetValue(float value);
};
