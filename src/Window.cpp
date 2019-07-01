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
#include "Widgets/ImGui/WorkAreaWindow.h"
#include "AppState.h"


#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Common/Logger.h"
#include <IconsFontAwesome5_c.h>

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
        mWindowWidth(DEFAULT_WINDOW_WIDTH),
        mWindowHeight(DEFAULT_WINDOW_HEIGHT),
        mName("CoconutCNC"),
        mAppState(state),
        mClearColor(0.75f,0.75f,0.75f),
        mDefaultFont(nullptr),
        mIconFont(nullptr),
        mFontSize(16.0f)
   	{
        debug("Window: Constructor");
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
            debug("Window: Size Changed to {}x{}",mWindowWidth,mWindowHeight);
			glfwGetFramebufferSize(mWindow, &mWindowWidth, &mWindowHeight);
        	glViewport(0,0,mWindowWidth,mWindowHeight);
            WindowSizeChanged = false;
		}

        glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, 0.0f);
        GLCheckError();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GLCheckError();

		DrawImGui();
		DrawGLWidgets();

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
		mWindow = glfwCreateWindow(mWindowWidth, mWindowHeight, mName.c_str(), nullptr,nullptr);

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

		glfwGetFramebufferSize(mWindow, &mWindowWidth, &mWindowHeight);
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
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Dockable Windows
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

        // When viewports are enabled we tweak WindowRounding/WindowBg so
        // platform windows can look identical to regular ones.

	   ImGuiStyle& style = ImGui::GetStyle();
	   if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	   {
		   style.WindowRounding = 0.0f;
		   style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	   }

		ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
        GLCheckError();
        LoadFonts();
		return true;
	}

    void Window::LoadFonts()
    {
        debug("Window: {}",__FUNCTION__);
        ImGuiIO& io = ImGui::GetIO();
        mDefaultFont = io.Fonts->AddFontFromFileTTF(DEFAULT_FONT_PATH, mFontSize);

        static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
        mIconFont = io.Fonts->AddFontFromFileTTF(FA_REGULAR_FONT_PATH, 13.0f, nullptr, icon_ranges);
    }

	bool Window::InitGL()
	{
        if(!gladLoadGL())
        {
			error("Window: Error initialising GLAD!\n");
			return false;
        }

        glViewport(0,0,mWindowWidth,mWindowHeight);

        debug("Window: OpenGL Version {}, Shader Version {}",
			glGetString(GL_VERSION),
			glGetString(GL_SHADING_LANGUAGE_VERSION));

        GLCheckError();

        mAppState->GetPreviewWindow().InitGL();

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

        WorkAreaWindow& pw = mAppState->GetPreviewWindow();
        GrblMachineModel& grbl = mAppState->GetGrblMachineModel();

        // Change Viewport

        pw.SetCameraTarget(grbl.GetMachinePosition());

        pw.InitViewMatrix();
        if (pw.PreviewSizeHasChanged())
        {
           pw.InitProjectionMatrix();
           pw.InitTexture();
           pw.InitDepthStencilBuffer();
           pw.BindFramebuffer();
        }

        float w = pw.GetContentAreaWidth();
		float h = pw.GetContentAreaHeight();
        glViewport(0,0,w,h);

        GLWidget::SetViewMatrix(mAppState->GetPreviewWindow().GetViewMatrix());
        GLWidget::SetProjectionMatrix(mAppState->GetPreviewWindow().GetProjectionMatrix());

        glBindFramebuffer(GL_FRAMEBUFFER, pw.GetFBO());

        glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, 0.0f);
        GLCheckError();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GLCheckError();

		for (GLWidget* widget : mGLWidgets)
		{
			if(widget->GetVisible())
			{
                widget->Update();
				widget->Draw();
			}
		}
        glDisable(GL_DEPTH_TEST);
        GLCheckError();

        glBindFramebuffer(GL_FRAMEBUFFER,0);
        GLCheckError();

        // Restore Viewport
        glViewport(0,0,mWindowWidth,mWindowHeight);
	}

	void
	Window::DrawImGui
	()
	{
        debug("Window: {}",__FUNCTION__);
        ImGuiIO& io = ImGui::GetIO();
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		// Rendering

        ImGui::PushFont(mDefaultFont);

        static bool p_open = true;
        static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &p_open, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		debug("Window {} ImGui Widgets", mImGuiWidgets.size());

		for (ImGuiWidget* widget : mImGuiWidgets)
		{
			if (widget->GetVisible())
			{
				widget->Draw();
			}
		}

        ImGui::PopFont();

		ImGui::End();




		// End Rendering
		ImGui::Render();
		glfwMakeContextCurrent(mWindow);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwMakeContextCurrent(mWindow);

        // Update and Render additional Platform Windows
		 // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		 //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		 if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		 {
			 GLFWwindow* backup_current_context = glfwGetCurrentContext();
			 ImGui::UpdatePlatformWindows();
			 ImGui::RenderPlatformWindowsDefault();
			 glfwMakeContextCurrent(backup_current_context);
		 }
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

    ImFont* Window::GetIconFont()
    {
        return mIconFont;
    }

    ImFont* Window::GetDeafaultFont()
    {
        return mDefaultFont;
    }
}
