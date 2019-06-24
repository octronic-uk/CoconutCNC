/*
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

#include "Window.h"

#define GL_SILENCE_DEPRECATION

#include "Widgets/GL/GLWidget.h"
#include "Widgets/ImGui/ImGuiWidget.h"
#include "AppState.h"


#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Logger.h"

using glm::ortho;
using std::cout;
using std::endl;

static bool WindowSizeChanged = false;
static bool WindowShouldClose = false;

void FramebufferSizeCallback(GLFWwindow*, int width, int height)
{
	debug("Window: {} {}x{}",__FUNCTION__, width, height);
	WindowSizeChanged = true;
}

void WindowShouldCloseCallback(GLFWwindow* window)
{
	WindowShouldClose = true;
}

void GlfwErrorCallback(int _errno, const char* errmsg)
{
	error("Window: GLFW Error Number {}\nMessage:\n{}", _errno ,errmsg);
}

namespace Coconut
{
	Window::Window(AppState* state) :
        mWidth(DEFAULT_WINDOW_WIDTH),
        mHeight(DEFAULT_WINDOW_HEIGHT),
        mName("CoconutCNC"),
        mAppState(state),
        mClearColor(0.75f,0.75f,0.75f),
        mDefaultFont(nullptr),
        mFontSize(16.0f),
        mCameraPosition(0.f,50.f,-50.f),
        mCameraTarget(0.f,0.f,0.f),
        mUpVector(0.f,1.f,0.f),
        mViewMatrix(mat4(1.0f)),
        mProjectionMatrix(mat4(1.0f)),
        mProjectionType(Perspective)
	{
        debug("Window: Constructor");

		InitProjectionMatrix();

        mViewMatrix = glm::lookAt(
            mCameraPosition, // the position of your camera, in world space
            mCameraTarget,   // where you want to look at, in world space
            mUpVector        // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
        );
	}

    Window::~Window
	()
	{
        debug("Window: Destructor");
		CleanUpImGui();
		if (mWindow)
		{
			glfwTerminate();
			mWindow = nullptr;
		}
        GLCheckError();
	}

    void Window::InitProjectionMatrix()
    {
        switch (mProjectionType)
        {
            case Ortho:
                // left, right, top, bottom, near, far
				mProjectionMatrix = ortho(
					0.f, (float)mWidth,
					0.f, (float)mHeight,
					0.1f, 100.f);
                break;
            case Perspective:
                // FOV, Aspect, Near, Far
				mProjectionMatrix = glm::perspective(
            		glm::radians(75.0f), (float)mWidth / (float)mHeight,
					0.1f, 100.0f);
                break;
        }
    }

	bool Window::Update()
	{
        debug("Window: {}",__FUNCTION__);

        glfwPollEvents();

		if(glfwWindowShouldClose(mWindow) || WindowShouldClose)
		{
            mAppState->SetLooping(false);
		}

		if (WindowSizeChanged)
		{
            debug("Window: Size Changed to {}x{}",mWidth,mHeight);
			glfwGetFramebufferSize(mWindow, &mWidth, &mHeight);
        	glViewport(0,0,mWidth,mHeight);
            InitProjectionMatrix();
            WindowSizeChanged = false;
		}

        glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, 0.0f);
        GLCheckError();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GLCheckError();

		DrawGLWidgets();
		DrawImGui();

		SwapBuffers();
        GLCheckError();
    	return true;
	}

	bool Window::Init()
	{
		if (!InitGLFW())
		{
			return false;
		}

		if (!InitGL())
		{
			return false;
		}

		if (!InitImGui())
		{
			return false;
		}

		return true;
	}

	bool Window::InitGLFW()
	{

		/* Initialize the library */
		if (!glfwInit())
		{
			return false;
		}

		/* Create a windowed mode window and its OpenGL context */
		//glfwWindowHint(GLFW_SAMPLES, 8);
		#ifdef WIN32
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		#endif
		#ifdef __APPLE__
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
		#endif
		#ifdef __linux__
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
		#endif
		mWindow = glfwCreateWindow(mWidth, mHeight, mName.c_str(), nullptr,nullptr);

		if (mWindow == nullptr)
		{
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(mWindow);

		glfwSetErrorCallback(GlfwErrorCallback);
		glfwSetFramebufferSizeCallback(mWindow, FramebufferSizeCallback);
		glfwSwapInterval(1);

        #ifdef __APPLE__
		glfwGetMonitorContentScale(glfwGetPrimaryMonitor(),&mDPIScaleX,&mDPIScaleY); //Requires GLFW >=3.3
        #endif

		glfwGetFramebufferSize(mWindow, &mWidth, &mHeight);
        glfwSetWindowCloseCallback(mWindow, WindowShouldCloseCallback);
        GLCheckError();
		return true;
	}

	bool Window::InitImGui()
	{
        const char* glsl_version = "#version 330 core";
		// Setup Dear ImGui binding
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |=
			ImGuiConfigFlags_DockingEnable; // Dockable Windows

		ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
        LoadDefaultFont();
        GLCheckError();
		return true;
	}

    void Window::LoadDefaultFont()
    {
        debug("Window: {}",__FUNCTION__);
        ImGuiIO& io = ImGui::GetIO();
        mDefaultFont = io.Fonts->AddFontFromFileTTF(DEFAULT_FONT_PATH, mFontSize);
    }

	bool Window::InitGL()
	{
		glewExperimental = GL_TRUE;
		GLenum glewInitResult = glewInit();

		if (glewInitResult != GLEW_OK)
		{
			return false;
		}

        glLineWidth(1.0f);
        glViewport(0,0,mWidth,mHeight);

        debug("Window: OpenGL Version {}, Shader Version {}",
			glGetString(GL_VERSION),
			glGetString(GL_SHADING_LANGUAGE_VERSION));

        GLCheckError();

		return true;
	}

	void Window::SwapBuffers()
	{
		if (mWindow != nullptr)
		{
			glfwSwapBuffers(mWindow);
        	GLCheckError();
		}
	}

	void
	Window::DrawGLWidgets
	()
	{
        debug("Window: {}",__FUNCTION__);
        debug("Window {} GL Widgets", mGLWidgets.size());

        GLWidget::SetViewMatrix(mViewMatrix);
        GLWidget::SetProjectionMatrix(mProjectionMatrix);

		for (GLWidget* widget : mGLWidgets)
		{
			if(widget->GetVisible())
			{
				widget->Draw();
			}
		}
        GLCheckError();
	}

	void
	Window::DrawImGui
	()
	{
        debug("Window: {}",__FUNCTION__);
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		// Rendering
        debug("Window {} ImGui Widgets", mImGuiWidgets.size());
		for (ImGuiWidget* widget : mImGuiWidgets)
		{
			if (widget->GetVisible())
			{
				widget->Draw();
			}
		}
		// End Rendering
		ImGui::Render();
		glfwMakeContextCurrent(mWindow);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwMakeContextCurrent(mWindow);
        ImGui::EndFrame();
        GLCheckError();
    }

    float Window::GetFontSize() const
    {
        return mFontSize;
    }

    void Window::SetFontSize(float fontSize)
    {
        mFontSize = fontSize;
    }

    void
    Window::CleanUpImGui
    ()
    {
        // Cleanup
        debug("Window: {}",__FUNCTION__);
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
	}

	void Window::AddImGuiWidget(ImGuiWidget* widget)
	{
        debug("Window: {}",__FUNCTION__);
		auto end = mImGuiWidgets.end();
		auto itr = find(mImGuiWidgets.begin(), end, widget);

		if (itr == end)
		{
			mImGuiWidgets.push_back(widget);
		}
	}

	void Window::RemoveImGuiWidget (ImGuiWidget* widget)
	{
        debug("Window: {}",__FUNCTION__);
		auto end = mImGuiWidgets.end();
		auto itr = find(mImGuiWidgets.begin(), end, widget);
		if (itr != end)
		{
			mImGuiWidgets.erase(itr);
		}
	}

	void Window::AddGLWidget (GLWidget* widget)
	{
        debug("Window: {}",__FUNCTION__);
		auto end = mGLWidgets.end();
		auto itr = find(mGLWidgets.begin(), end, widget);

		if (itr == end)
		{
			mGLWidgets.push_back(widget);
		}
	}

	void Window::RemoveGLWidget(GLWidget* widget)
	{
        debug("Window: {}",__FUNCTION__);
		auto end = mGLWidgets.end();
		auto itr = find(mGLWidgets.begin(), end, widget);
		if (itr != end)
		{
			mGLWidgets.erase(itr);
		}
	}

    vector<ImGuiWidget*>& Window::GetImGuiWidgetsVector()
    {
        return mImGuiWidgets;
    }
}
