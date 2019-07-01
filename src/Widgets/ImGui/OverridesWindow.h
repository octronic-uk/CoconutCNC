#pragma once

#include "ImGuiWidget.h"

namespace Coconut
{
    class OverridesWindow : public ImGuiWidget
    {
    public:
        OverridesWindow(AppState* proj);
        ~OverridesWindow() override;
        void Draw() override;
    };
}
