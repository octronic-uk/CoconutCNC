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
        void AddLineVertex(const GLWidgetVertex& v);
        void AddLineVertices(const vector<GLWidgetVertex>& v);

        void AddTriangleVertex(const GLWidgetVertex& v);
        void AddTriangleVertices(const vector<GLWidgetVertex>& v);

        void AddPointVertex(const GLWidgetVertex& v);
        void AddPointVertices(const vector<GLWidgetVertex>& v);

        virtual void SetShader();
        bool InitShader();
        bool InitLineBuffers();
        bool InitTriangleBuffers();
        bool InitPointBuffers();

        void ClearLineVertexBuffer();
        void ClearTriangleVertexBuffer();
        void ClearPointVertexBuffer();

        void SubmitLineVertexBuffer();
        void SubmitTriangleVertexBuffer();
        void SubmitPointVertexBuffer();

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

        GLuint mPointVao;
        GLuint mPointVbo;
        vector<GLWidgetVertex> mPointVertexBuffer;


        GLuint mShaderProgram;
        string mVertexShaderSource;
        string mFragmentShaderSource;
        GLint mModelUniform;
        GLint mViewUniform;
        GLint mProjectionUniform;

    };
}
