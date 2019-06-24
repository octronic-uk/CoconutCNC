#pragma once

#include "../../GLHeader.h"
#include "../Widget.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>

using glm::vec3;
using glm::mat4;
using std::vector;
using std::string;

namespace Coconut
{
    struct GLWidgetVertex
    {
        vec3 Position;
        vec3 Color;
    };

    class GLWidget : public Widget
    {
    public:
        GLWidget(AppState* project, string name, bool visible = true);
        virtual ~GLWidget() override;

        virtual bool Init();
        virtual void Update() = 0;
        virtual void Draw() override;

        void SetPosition(const vec3&);
        vec3 GetPosition();

        static void SetViewMatrix(const mat4&);
        static void SetProjectionMatrix(const mat4&);
    protected:
        void AddLineVertex(const GLWidgetVertex& lv);
        void AddTriangleVertex(const GLWidgetVertex& lv);

        virtual void SetShader();
        bool InitShader();
        bool InitLineBuffers();
        bool InitTriangleBuffers();

        void ClearLineVertexBuffer();
        void ClearTriangleVertexBuffer();

        void SubmitLineVertexBuffer();
        void SubmitTriangleVertexBuffer();

    protected:
        static mat4 ViewMatrix;
        static mat4 ProjectionMatrix;

        mat4 mModelMatrix;

        GLuint mLineVao;
        GLuint mLineVbo;
        vector<GLWidgetVertex> mLineVertexBuffer;

        GLuint mTriangleVao;
        GLuint mTriangleVbo;
        vector<GLWidgetVertex> mTriangleVertexBuffer;

        GLuint mShaderProgram;
        string mVertexShaderSource;
        string mFragmentShaderSource;
        GLint mModelUniform;
        GLint mViewUniform;
        GLint mProjectionUniform;

    };
}
