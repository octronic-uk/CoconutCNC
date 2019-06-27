#include "AppState.h"
#include "Common/Logger.h"

namespace Coconut
{
	AppState::AppState(int argc, char** argv) :
        mLooping(true),
        mArgc(argc),
    	mArgv(argv),
      	mWindow(this),
        // Models
        mSettingsModel(this),
        mSerialPortModel(this),
        mGrblMachineModel(this),
        // ImGui Widgets
        mConsoleWindow(this),
        mControlWindow(this),
        mGCodeTableWindow(this),
        mJogWindow(this),
        mMenuBar(this),
        mOverridesWindow(this),
        mPreviewWindow(this),
        mSettingsWindow(this),
        mStateWindow(this),
		// GL Widgets
        mGridDrawer(this),
        mToolDrawer(this)
	{
		debug("AppState: Constructor");
    }

    bool AppState::Init()
    {
		debug("AppState: Init");
        if (!mWindow.Init())       return false;
        if (!CreateImGuiWidgets()) return false;
        if (!CreateGLWidgets())    return false;
        return true;
    }

    bool AppState::CreateImGuiWidgets()
    {
		debug("AppState: CreateImGuiWidgets");
        mWindow.AddImGuiWidget(&mConsoleWindow);
        mWindow.AddImGuiWidget(&mControlWindow);
        mWindow.AddImGuiWidget(&mGCodeTableWindow);
        mWindow.AddImGuiWidget(&mJogWindow);
        mWindow.AddImGuiWidget(&mMenuBar);
        mWindow.AddImGuiWidget(&mOverridesWindow);
        mWindow.AddImGuiWidget(&mPreviewWindow);
        mWindow.AddImGuiWidget(&mSettingsWindow);
        mWindow.AddImGuiWidget(&mStateWindow);
        return true;
    }

    bool AppState::CreateGLWidgets()
    {
		debug("AppState: CreateGLWidgets");

        if (!mGridDrawer.Init()) return false;
        mWindow.AddGLWidget(&mGridDrawer);

        if(!mToolDrawer.Init()) return false;
        mWindow.AddGLWidget(&mToolDrawer);

        return true;
    }

    SettingsModel& AppState::GetSettingsModel()
    {
        return mSettingsModel;
    }

    SerialPortModel& AppState::GetSerialPortModel()
    {
        return mSerialPortModel;
    }

    GCodeFileModel& AppState::GetGCodeFileModel()
    {
       return mGCodeFileModel;
    }

    WorkAreaWindow& AppState::GetPreviewWindow()
    {
        return mPreviewWindow;
    }

    GrblMachineModel& AppState::GetGrblMachineModel()
    {
       return mGrblMachineModel;
    }

    bool AppState::GetLooping() const
    {
        return mLooping;
    }

    void AppState::SetLooping(bool looping)
    {
        mLooping = looping;
    }

    bool AppState::Run()
    {
		debug("AppState: Run");
        while (mLooping)
        {
            mWindow.Update();
        }
        mSettingsModel.SaveSettingsFile();
        if (mGrblMachineModel.IsWorkThreadRunning())
        {
        	mGrblMachineModel.JoinWorkThread();
        }
        mSerialPortModel.CloseSerialPort();
        return true;
    }

    bool AppState::OpenGCodeFile(string path)
	{
		return false;
	}

    void AppState::CloseGCodeFile()
	{
	}

    Window& AppState::GetWindow()
    {
        return mWindow;
    }
}
