#include "AppState.h"
#include "Logger.h"

namespace Coconut
{
	AppState::AppState(int argc, char** argv) :
        mLooping(true),
        mArgc(argc),
    	mArgv(argv),
      	mWindow(Window(this)),
        mConsoleWindow(ConsoleWindow(this)),
        mControlWindow(ControlWindow(this)),
        mJogWindow(JogWindow(this)),
        mMenuBar(MenuBar(this)),
        mOverridesWindow(OverridesWindow(this)),
        mStateWindow(StateWindow(this)),
        mGrid(GridDrawer(this))
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
        mWindow.AddImGuiWidget(&mJogWindow);
        mWindow.AddImGuiWidget(&mMenuBar);
        mWindow.AddImGuiWidget(&mOverridesWindow);
        mWindow.AddImGuiWidget(&mStateWindow);
        return true;
    }

    bool AppState::CreateGLWidgets()
    {
		debug("AppState: CreateGLWidgets");
        if (!mGrid.Init()) return false;
        mWindow.AddGLWidget(&mGrid);
        return true;
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
        return true;
    }

    bool AppState::OpenGCodeFile(string path)
	{
		return false;
	}

    void AppState::CloseGCodeFile()
	{
	}

    Window* AppState::GetWindow()
    {
        return &mWindow;
    }
}
