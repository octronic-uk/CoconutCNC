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
    private:
        int mJogFeedRate;
        float mJogStepValue;
        float mJogAbsoluteX;
        float mJogAbsoluteY;
        float mJogAbsoluteZ;
    };
}
