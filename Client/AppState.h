#pragma once

#include "Window.h"

#include "Widgets/ImGui/ConsoleWindow.h"
#include "Widgets/ImGui/ControlWindow.h"
#include "Widgets/ImGui/JogWindow.h"
#include "Widgets/ImGui/MenuBar.h"
#include "Widgets/ImGui/OverridesWindow.h"
#include "Widgets/ImGui/StateWindow.h"

#include "Widgets/GL/GridDrawer.h"

namespace Coconut
{
	class AppState
	{
	public:
		AppState(int argc, char** argv);
        bool Init();
        bool Run();

        bool GetLooping() const;
        void SetLooping(bool looping);

        bool OpenGCodeFile(string path);
        void CloseGCodeFile();

        Window* GetWindow();

    protected:
        bool CreateImGuiWidgets();
        bool CreateGLWidgets();

    private:
        bool mLooping;
        int mArgc;
        char** mArgv;
        Window mWindow;
        // ImGui Widgets
        ConsoleWindow mConsoleWindow;
        ControlWindow mControlWindow;
        JogWindow mJogWindow;
        MenuBar mMenuBar;
        OverridesWindow mOverridesWindow;
        StateWindow mStateWindow;
        // GL Widgets
        GridDrawer mGrid;
	};
}
