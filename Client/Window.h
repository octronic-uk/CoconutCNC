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
#include "Deps/ImGui/imgui.h"
#include "Deps/ImGui/imgui_impl_glfw.h"
#include "Deps/ImGui/imgui_impl_opengl3.h"

#define DEFAULT_FONT_PATH "droid-sans/DroidSans.ttf"

namespace Coconut
{
	class GLWidget;
	class ImGuiWidget;
    class AppState;

    enum ProjectionType
    {
        Ortho,
        Perspective
    };

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
        void InitProjectionMatrix();

        float GetFontSize() const;
        void SetFontSize(float fontSize);

    	vector<ImGuiWidget*>& GetImGuiWidgetsVector();
    protected:

        void LoadDefaultFont();

    private:
        GLFWwindow* mWindow;
        int mWidth;
        vector<GLWidget*> mGLWidgets;
        vector<ImGuiWidget*> mImGuiWidgets;
        float mUiFontSize;
        float mMonoFontSize;
        float mDPIScaleX, mDPIScaleY;
        int mHeight;
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
        float mFontSize;
        vec3 mClearColor;
        vec3 mCameraPosition;
        vec3 mCameraTarget;
        vec3 mUpVector;
        mat4 mViewMatrix;
        mat4 mProjectionMatrix;
        ProjectionType mProjectionType;
	};
}
