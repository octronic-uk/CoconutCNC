#pragma once

#include "ImGuiWidget.h"

namespace Coconut
{
    class ConsoleWindow : public ImGuiWidget
    {
    public:
        ConsoleWindow(AppState* proj);
        ~ConsoleWindow() override;
        void Draw() override;
    };
}
