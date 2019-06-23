#pragma once

#include "ImGuiWidget.h"

namespace Coconut
{
    class ControlWindow : public ImGuiWidget
    {
    public:
        ControlWindow(AppState* proj);
        ~ControlWindow() override;
        void Draw() override;
    };
}
