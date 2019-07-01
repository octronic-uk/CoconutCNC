#pragma once

#include "ImGuiWidget.h"

#define MDI_BUFFER_SIZE 128


namespace Coconut
{
    enum ConsoleLineType
    {
        Command,
        Response
    };

	struct ConsoleLine
    {
       string Data;
       ConsoleLineType Type;
    };

    class ConsoleWindow : public ImGuiWidget
    {
    public:
        ConsoleWindow(AppState* proj);
        ~ConsoleWindow() override;
        void Draw() override;
        void PushConsoleLine(const ConsoleLine& c);
        ImVec4 mCommandTextColor;
        ImVec4 mResponseTextColor;
    private:
        char mMdiBuffer[MDI_BUFFER_SIZE];
        vector<ConsoleLine> mConsoleLineBuffer;
    };
}
