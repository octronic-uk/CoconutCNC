#pragma once

#include "ImGuiWidget.h"

namespace Coconut
{
    class JogWindow : public ImGuiWidget
    {
    public:
        JogWindow(AppState* proj);
        ~JogWindow() override;
        void Draw() override;
    };
}
