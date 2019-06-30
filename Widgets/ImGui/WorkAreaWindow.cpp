#include "WorkAreaWindow.h"
#include "../../AppState.h"
#include "../../Window.h"

#include <glm/gtc/matrix_transform.hpp>

using glm::ortho;
using glm::perspective;

namespace Coconut
{
    WorkAreaWindow::WorkAreaWindow(AppState* project)
        : ImGuiWidget(project, "Work Area"),
          mFBO(0),
          mTexture(0),
          mDepthStencilBuffer(0),
          mLastContentAreaSize(ImVec2(10.0,10.0)),
          mContentAreaSize(ImVec2(10.0,10.0)),
          mCameraPosition(0.f,50.f,-50.f),
          mCameraSpeedScale(50.f),
          mCameraPitchMax(glm::radians(45.f)),
          mCameraTarget(0.f,0.f,0.f),
          mCameraRadius(200.f),
          mCameraRadiusMin(10.f),
          mCameraPitch(0),
          mCameraYaw(0.f),
          mUpVector(0.f,0.f,1.f),
          mViewMatrix(mat4(1.0f)),
          mProjectionMatrix(mat4(1.0f)),
          mNearClip(.1f),
          mFarClip(1000.f),
          mProjectionType(Perspective),
          mFollowTool(true)
    {
        UpdateCameraPosition();
    }

    WorkAreaWindow::~WorkAreaWindow ()
	{
        if (mDepthStencilBuffer > 0)
        {
            glDeleteRenderbuffers(1,&mDepthStencilBuffer);
            mDepthStencilBuffer = 0;
        }
        if (mTexture > 0)
        {
            glDeleteTextures(1, &mTexture);
        }
        if (mFBO > 0)
        {
           glDeleteFramebuffers(1,&mFBO);
        }
        GLCheckError();
    }

    bool WorkAreaWindow::InitGL()
    {
        if (!InitFramebuffer())
        {
            return false;
        }

        if(!InitTexture())
        {
            return false;
        }

        if (!InitDepthStencilBuffer())
        {
            return false;
        }

        BindFramebuffer();

        return true;
    }

    bool WorkAreaWindow::InitFramebuffer()
	{
		// Create Window Framebuffer
        glGenFramebuffers(1, &mFBO);
        if (mFBO == 0)
        {
            error("PreviewWindow: Error creating framebuffer");
            return false;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
        {
            error("Window: Unable to create window framebuffer");
        	GLCheckError();
        	glBindFramebuffer(GL_FRAMEBUFFER,0);
			return false;
        }

        GLCheckError();
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        return true;
    }

    bool WorkAreaWindow::InitDepthStencilBuffer()
    {
       if (mDepthStencilBuffer > 0)
       {
           glDeleteRenderbuffers(1,&mDepthStencilBuffer);
           mDepthStencilBuffer = 0;
       }

      	glGenRenderbuffers(1,&mDepthStencilBuffer);

        if (glGenRenderbuffers == 0)
        {
            error("WorkAreaWindow: Unable to create depth buffer");
            GLCheckError();
            return false;
        }

        glBindRenderbuffer(GL_RENDERBUFFER, mDepthStencilBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
        	GetContentAreaWidth(), GetContentAreaHeight());

		glBindRenderbuffer(GL_RENDERBUFFER, 0);

       return true;
    }

    bool WorkAreaWindow::InitTexture()
    {
        // Clear Existing Texture
        if (mTexture != 0)
        {
            glDeleteTextures(1,&mTexture);
            mTexture = 0;
        }

        // Create Target Texture
        glGenTextures(1, &mTexture);
        if (mTexture == 0)
        {
            error("PreviewWindow: Unable to create texture");
            GLCheckError();
            mTexture = 0;
            return false;
        }

        // Set Parameters
        glBindTexture(GL_TEXTURE_2D, mTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GetContentAreaWidth(), GetContentAreaHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Unbind
        glBindTexture(GL_TEXTURE_2D,0);

        return true;
    }

    bool WorkAreaWindow::BindFramebuffer()
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mDepthStencilBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        return true;
    }

    void WorkAreaWindow::InitViewMatrix()
    {
        mViewMatrix = glm::lookAt(
            mCameraPosition, // the position of your camera, in world space
            mCameraTarget,   // where you want to look at, in world space
            mUpVector        // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
        );
    }

     void WorkAreaWindow::InitProjectionMatrix()
    {
        switch (mProjectionType)
        {
            case Ortho:
                // left, right, top, bottom, near, far
				mProjectionMatrix = ortho(
					0.f, (float)mContentAreaSize.x,
					0.f, (float)mContentAreaSize.y,
					mNearClip, mFarClip);
                break;
            case Perspective:
                // FOV, Aspect, Near, Far
				mProjectionMatrix = perspective(
            		glm::radians(45.0f),
					(float)mContentAreaSize.x / (float)mContentAreaSize.y,
					mNearClip, mFarClip);
                break;
        }
    }

     void WorkAreaWindow::UpdateCameraPosition()
     {
        vec3 mpos = mAppState->GetGrblMachineModel().GetMachinePosition();

        mCameraPosition.x = (cos(mCameraYaw)*cos(mCameraPitch))*mCameraRadius;
        mCameraPosition.y = (sin(mCameraYaw)*cos(mCameraPitch))*mCameraRadius;
        mCameraPosition.z = sin(mCameraPitch)*mCameraRadius;

        InitViewMatrix();
        //info("WorkAreaDrawer: Cam at {},{},{}",mCameraPosition.x, mCameraPosition.y, mCameraPosition.z);
     }

    void WorkAreaWindow::Draw()
    {
        mLastContentAreaSize.x = mContentAreaSize.x;
        mLastContentAreaSize.y = mContentAreaSize.y;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2(0.0f,0.0f));
        ImGui::Begin(mName.c_str(), &mVisible);
        mContentAreaSize = ImGui::GetContentRegionAvail();
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Image((void*)(intptr_t)mTexture,mContentAreaSize,UV1, UV2);
        if (ImGui::IsItemHovered())
		{
            if(ImGui::IsMouseDragging())
            {
                ImVec2 delta = ImGui::GetIO().MouseDelta;
                mCameraPitch += delta.y / mCameraSpeedScale;
                mCameraYaw += delta.x / mCameraSpeedScale;
            }

            mCameraRadius += io.MouseWheel / mCameraSpeedScale;

            // Clamping

            if (mCameraPitch > mCameraPitchMax) mCameraPitch = mCameraPitchMax;
            else if (mCameraPitch < -mCameraPitchMax) mCameraPitch = -mCameraPitchMax;

            if (mCameraRadius < mCameraRadiusMin) mCameraRadius = mCameraRadiusMin;

            debug("WorkAreaWindow: pitch {} yaw {} radius {}", mCameraPitch, mCameraYaw, mCameraRadius);
		}

        UpdateCameraPosition();

		ImGui::End();
        ImGui::PopStyleVar();
    }

    int WorkAreaWindow::GetContentAreaWidth()
    {
		return mContentAreaSize.x;
    }

    int WorkAreaWindow::GetContentAreaHeight()
    {
		return mContentAreaSize.y;
    }

    bool WorkAreaWindow::PreviewSizeHasChanged()
    {
        return mContentAreaSize.x != mLastContentAreaSize.x ||
                mContentAreaSize.y != mLastContentAreaSize.y;
    }

    GLuint WorkAreaWindow::GetFBO()
    {
       return mFBO;
    }

    mat4 WorkAreaWindow::GetViewMatrix()
    {
        return mViewMatrix;
    }

    mat4 WorkAreaWindow::GetProjectionMatrix()
    {
        return mProjectionMatrix;
    }

	ImVec2 WorkAreaWindow::UV1 = ImVec2(0,1);
	ImVec2 WorkAreaWindow::UV2 = ImVec2(1,0);

}

