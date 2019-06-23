#include "GLWidget.h"

#include "../../Logger.h"
#include "../../AppState.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Coconut
{
    mat4 GLWidget::ViewMatrix = mat4(1.0f);
    mat4 GLWidget::ProjectionMatrix = mat4(1.0f);

    GLWidget::GLWidget(AppState* project, string name, bool visible)
        : Widget (project, name, visible),
          mModelMatrix(mat4(1.0f)),
          mVao(0),
          mVbo(0),
          mShaderProgram(0)
    {
        debug("GLWidget: Constructor");
    }

    bool GLWidget::Init()
    {
        debug("GLWidget: {}",__FUNCTION__);
        SetShader();

        if (!InitShader())
        {
            return false;
        }

        if (!InitVaoVbo())
        {
            return false;
        }

        return true;
    }

    GLWidget::~GLWidget()
    {
        debug("GLWidget: Destructor");

        if (mVao > 0)
        {
            glDeleteVertexArrays(1,&mVao);
        }

        if (mVbo > 0)
        {
            glDeleteBuffers(1,&mVbo);
        }

        if (mShaderProgram > 0)
        {
            glDeleteProgram(mShaderProgram);
        }
        GLCheckError();
    }

    bool GLWidget::InitVaoVbo()
    {
        debug("GLWidget: {}", __FUNCTION__);

        // VAO
        glGenVertexArrays(1,&mVao);
        if (mVao < 0)
        {
            error("GLWidget: Error creating VAO");
            return false;
        }
        glBindVertexArray(mVao);

        // VBO
        glGenBuffers(1,&mVbo);
        if (mVbo < 0)
        {
            error("GLWidget: Error creating VBO");
            return false;
        }
        glBindBuffer(GL_ARRAY_BUFFER,mVbo);

        // Vertex Position Attributes
        glVertexAttribPointer(
            0, 3, GL_FLOAT, GL_FALSE,
            static_cast<GLint>(sizeof(GLWidgetVertex)),
            static_cast<GLvoid*>(0)
        );
        glEnableVertexAttribArray(0);

        // Vertex Color Attributes
        glVertexAttribPointer(
            1, 3, GL_FLOAT, GL_FALSE,
            static_cast<GLint>(sizeof(GLWidgetVertex)),
            (GLvoid*)(sizeof(float)*3)
        );
        glEnableVertexAttribArray(1);

        GLCheckError();

        glBindVertexArray(0);

        //  Final Check
        if (mVao != -1 && mVbo != -1)
        {
            debug("GLWidget: Shader Init Successful");
            return true;
        }
        else
        {
           error("GLWidget: VAO/VBO Error VAO:{} VBO:{}",mVao,mVbo);
        }
        return false;
    }

    void GLWidget::SetPosition(const vec3& pos)
    {
        mModelMatrix = glm::translate(mat4(1.0f),pos);
    }

    vec3 GLWidget::GetPosition()
    {
       return vec3(mModelMatrix[3]);
    }

    void GLWidget::SetViewMatrix(const mat4& view)
    {
        debug("GLWidget: {}", __FUNCTION__);
        ViewMatrix = view;
    }

    void GLWidget::Draw()
    {
        debug("GLWidget: {}", __FUNCTION__);
        // set proj/view matricies here
        if (!mVertexBuffer.empty())
        {
            // Enable shader program
            glUseProgram(mShaderProgram);
        	GLCheckError();

            // Vertex Array
            glBindVertexArray(mVao);
        	GLCheckError();

            //glBindBuffer(GL_ARRAY_BUFFER, mVbo);
            // Set the projection matrix
            if (mModelUniform == -1)
            {
                error("GLWidget: ModelUniform not found in ShaderProgram");
                return;
            }
            else
            {
                glUniformMatrix4fv(mModelUniform, 1, GL_FALSE, glm::value_ptr(mModelMatrix));
        		GLCheckError();
            }
			// Set the view matrix
            if (mViewUniform == -1)
            {
                error("GLWidget: ViewUniform not found in ShaderProgram");
                return;
            }
            else
            {
                glUniformMatrix4fv(mViewUniform, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
        		GLCheckError();
            }
            // Set the projection matrix
            if (mProjectionUniform == -1)
            {
                error("GLWidget: ProjectionUniform not found in ShaderProgram");
                return;
            }
            else
            {
                glUniformMatrix4fv(mProjectionUniform, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
        		GLCheckError();
            }
            // Draw
            debug("GLWidget: Drawing {} lines",mVertexBuffer.size()/2);
            glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(mVertexBuffer.size()));
        	GLCheckError();

            glBindVertexArray(0);
        	GLCheckError();
        }
    }

    void GLWidget::SetShader()
    {
        debug("GLWidget: {}", __FUNCTION__);
        mVertexShaderSource =
            "#version 330 core\n"
            "\n"
            "layout (location = 0) in vec3 in_position;\n"
            "layout (location = 1) in vec3 in_color;\n"
            "\n"
            "out vec3 Color;\n"
            "\n"
            "uniform mat4 model;\n"
            "uniform mat4 view;\n"
            "uniform mat4 projection;\n"
            "\n"
            "void main () {\n"
            "    gl_Position = projection * view * model * "
            "        vec4(in_position.x, in_position.y, in_position.z, 1.0);\n"
            "    Color = in_color;\n"
            "}";

        mFragmentShaderSource =
            "#version 330 core\n"
            "\n"
            "in vec3  Color;\n"
            "\n"
            "out vec4 FragColor;\n"
            "\n"
            "void main() {\n"
            "    FragColor = vec4(Color,1.0);\n"
            "}";
    }

    bool
    GLWidget::InitShader()
    {
        debug("GLWidget: {}", __FUNCTION__);
        GLuint vertexShader = 0;
        GLuint fragmentShader = 0;

        // Compile shaders
        GLint success;
        GLchar infoLog[512];

        // Vertex Shader
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char *vSource = mVertexShaderSource.c_str();
        glShaderSource(vertexShader, 1, &vSource, nullptr);
        glCompileShader(vertexShader);

        // Print compile errors if any
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            error("GLWidget: Vertex Shader Error {}", infoLog);
            return false;
        }

        // Fragment Shader
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char *fSource = mFragmentShaderSource.c_str();
        glShaderSource(fragmentShader, 1, &fSource, nullptr);
        glCompileShader(fragmentShader);

        // Print compile errors if any
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            error("GLWidget: Fragment Shader Error {}", infoLog);
            return false;

        }

        // Shader Program
        mShaderProgram = glCreateProgram();
        glAttachShader(mShaderProgram, vertexShader);
        glAttachShader(mShaderProgram, fragmentShader);
        glLinkProgram(mShaderProgram);

        // Print linking errors if any
        glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(mShaderProgram, 512, nullptr, infoLog);
            error("GLWidget: Shader Linking Error {}", infoLog);
            return false;

        }

        // Delete the shaders 'source objects' as they're linked into our program
        // now and no longer necessery
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Get Uniform Locations
        mModelUniform = glGetUniformLocation(mShaderProgram, "model");
        mViewUniform = glGetUniformLocation(mShaderProgram,"view");
        mProjectionUniform = glGetUniformLocation(mShaderProgram, "projection");

        GLCheckError();

        if (mModelUniform != -1 && mViewUniform != -1 && mProjectionUniform != -1)
        {
        	return true;
        }
        else
        {
       		error("GLWidget: Uniform Error M:{} V:{} P:{}",
                  mModelUniform, mViewUniform, mProjectionUniform);
            return false;
        }
    }

    void GLWidget::SetProjectionMatrix(const mat4& projection)
    {
        debug("GLWidget: {}", __FUNCTION__);
        ProjectionMatrix = projection;
    }

    void GLWidget::AddVertex(const GLWidgetVertex& lv)
    {
        mVertexBuffer.push_back(lv);
    }
}
