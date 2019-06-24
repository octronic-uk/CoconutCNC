#pragma once

#include "ImGuiWidget.h"

#define MDI_BUFFER_SIZE 128

namespace Coconut
{
    class ConsoleWindow : public ImGuiWidget
    {
    public:
        ConsoleWindow(AppState* proj);
        ~ConsoleWindow() override;
        void Draw() override;
    private:
        char mMdiBuffer[MDI_BUFFER_SIZE];
    };
}
