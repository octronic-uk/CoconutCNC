#include "AppState.h"
#include "Common/Logger.h"

#include <thread>

using std::this_thread::yield;

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
        mGCodeFileModel(this),
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
        mToolDrawer(this),
        mOriginDrawer(this),
        mGCodeDrawer(this)
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

        if (!mOriginDrawer.Init()) return false;
        mWindow.AddGLWidget(&mOriginDrawer);

        if (!mGCodeDrawer.Init()) return false;
        mWindow.AddGLWidget(&mGCodeDrawer);

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

    ConsoleWindow& AppState::GetConsoleWindow()
    {
       return mConsoleWindow;
    }

    GrblMachineModel& AppState::GetGrblMachineModel()
    {
        return mGrblMachineModel;
    }

    GCodeDrawer& AppState::GetGCodeDrawer()
    {
        return mGCodeDrawer;
    }

    ToolDrawer& AppState::GetToolDrawer()
    {
       return mToolDrawer;
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
            yield();
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
