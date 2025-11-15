#pragma once

#include "IWidget.h"
#include "Button.h"
#include "Label.h"
#include <vector>
#include <memory>

/// <summary>
/// Widget container pentru meniuri
/// </summary>
class Menu : public IWidget
{
private:
    std::vector<std::unique_ptr<IWidget>> m_Widgets;
    bool m_IsActive;

public:
    Menu();

    void Draw() override;
    void Update() override;
    bool IsHovered() const override;
    bool IsActive() const override;
    void SetActive(bool active) override;

    /// <summary>
    /// Adaugă un widget în meniu
    /// </summary>
    void AddWidget(std::unique_ptr<IWidget> widget);

    /// <summary>
    /// Șterge toate widget-urile
    /// </summary>
    void Clear();
};
