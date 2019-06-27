#pragma once

#include "ImGuiWidget.h"
#include "../../GLHeader.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

using glm::vec3;
using glm::mat4;

namespace Coconut
{
    enum ProjectionType
    {
        Ortho,
        Perspective
    };

    class WorkAreaWindow : public ImGuiWidget
    {
    public:
        WorkAreaWindow(AppState* proj);
        ~WorkAreaWindow() override;
        void Draw() override;

		bool InitGL();
        bool InitFramebuffer();
        bool InitTexture();
        void InitViewMatrix();
        void InitProjectionMatrix();

   	 	bool BindFramebufferTexture();

        int GetContentAreaWidth();
        int GetContentAreaHeight();
        bool PreviewSizeHasChanged();

        GLuint GetTexture();
        void   SetTexture(GLuint t);

        GLuint GetFBO();
        void   SetFBO(GLuint t);

        mat4 GetViewMatrix();
        mat4 GetProjectionMatrix();

    private:

        GLuint mFBO;
        GLuint mTexture;

        vec3 mCameraPosition;
        vec3 mCameraTarget;
        vec3 mUpVector;
        mat4 mViewMatrix;
        mat4 mProjectionMatrix;
        ProjectionType mProjectionType;

        ImVec2 mLastContentAreaSize;
        ImVec2 mContentAreaSize;
		static ImVec2 UV1;
		static ImVec2 UV2;
    };
}
