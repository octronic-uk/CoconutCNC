#pragma once

#include "Window.h"

// Models
#include "Model/Settings/SettingsModel.h"

// ImGui Widgets
#include "Widgets/ImGui/ConsoleWindow.h"
#include "Widgets/ImGui/ControlWindow.h"
#include "Widgets/ImGui/GCodeTableWindow.h"
#include "Widgets/ImGui/JogWindow.h"
#include "Widgets/ImGui/MenuBar.h"
#include "Widgets/ImGui/OverridesWindow.h"
#include "Widgets/ImGui/SettingsWindow.h"
#include "Widgets/ImGui/StateWindow.h"

// GL Widgets
#include "Widgets/GL/GridDrawer.h"
#include "Widgets/GL/ToolDrawer.h"

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

        SettingsModel* GetSettingsModel();

    protected:
        bool CreateImGuiWidgets();
        bool CreateGLWidgets();

    private:
        bool mLooping;
        int mArgc;
        char** mArgv;
        Window mWindow;
        // Model
        SettingsModel mSettingsModel;
        // ImGui Widgets
        ConsoleWindow mConsoleWindow;
        ControlWindow mControlWindow;
		GCodeTableWindow mGCodeTableWindow;
        JogWindow mJogWindow;
        MenuBar mMenuBar;
        OverridesWindow mOverridesWindow;
        SettingsWindow mSettingsWindow;
        StateWindow mStateWindow;
        // GL Widgets
        GridDrawer mGridDrawer;
        ToolDrawer mToolDrawer;
	};
}
