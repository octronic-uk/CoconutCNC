#pragma once

#include "GLHeader.h"

#include <vector>
#include <string>
#include <glm/glm.hpp>

using glm::vec3;
using glm::mat4;
using std::string;
using std::vector;

// Maintain include order for GL
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#define DEFAULT_FONT_PATH "Fonts/DroidSans/DroidSans.ttf"
#define FA_REGULAR_FONT_PATH "Fonts/FontAwesome/fa-regular-400.ttf"
#define FA_SOLID_FONT_PATH "Fonts/FontAwesome/fa-solid-900.ttf"

#define DEFAULT_WINDOW_WIDTH 1280
#define DEFAULT_WINDOW_HEIGHT 720

namespace Coconut
{
	class GLWidget;
	class ImGuiWidget;
    class AppState;

	class Window
	{

	public:
		Window(AppState* state);
        ~Window();

		bool Update();

        void AddGLWidget(GLWidget* widget);
        void RemoveGLWidget(GLWidget* widget);

        void AddImGuiWidget(ImGuiWidget* widget);
        void RemoveImGuiWidget(ImGuiWidget* widget);

        bool Init();

        float GetFontSize() const;
        void SetFontSize(float fontSize);
        ImFont* GetIconFont();
        ImFont* GetDeafaultFont();

    	vector<ImGuiWidget*>& GetImGuiWidgetsVector();
    protected:

        void LoadFonts();

    private:
        GLFWwindow* mWindow;
        int mWindowWidth;
        int mWindowHeight;
        vector<GLWidget*> mGLWidgets;
        vector<ImGuiWidget*> mImGuiWidgets;
        float mDPIScaleX, mDPIScaleY;
        string mName;

        void SwapBuffers();
        bool InitGLFW();
        bool InitGL();
        bool InitImGui();
        void CleanUpImGui();
        void SetTheme();
        void SetFont();
		void DrawGLWidgets();
		void DrawImGui();
        AppState* mAppState;
        ImFont* mDefaultFont;
        ImFont* mIconFont;
        float mFontSize;
        vec3 mClearColor;
	};
}
