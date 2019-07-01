#include "GLWidget.h"

#include "../../Common/Logger.h"
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
          mLineVao(0),
          mLineVbo(0),
          mTriangleVao(0),
          mTriangleVbo(0),
          mPointVao(0),
          mPointVbo(0),
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

        if (!InitLineBuffers())
        {
            return false;
        }

        if (!InitTriangleBuffers())
        {
            return false;
        }

        if (!InitPointBuffers())
        {
            return false;
    	}

        return true;
    }

    GLWidget::~GLWidget()
    {
        debug("GLWidget: Destructor");

        // Line

        if (mLineVao > 0)
        {
            glDeleteVertexArrays(1,&mLineVao);
        }

        if (mLineVbo > 0)
        {
            glDeleteBuffers(1,&mLineVbo);
        }

        // Triangle

        if (mTriangleVao > 0)
        {
            glDeleteVertexArrays(1,&mTriangleVao);
        }

        if (mTriangleVbo > 0)
        {
            glDeleteBuffers(1,&mTriangleVbo);
        }

        // Point

        if (mPointVao > 0)
        {
            glDeleteVertexArrays(1,&mPointVao);
        }

        if (mPointVbo > 0)
        {
            glDeleteBuffers(1,&mPointVbo);
        }

		// Shader

        if (mShaderProgram > 0)
        {
            glDeleteProgram(mShaderProgram);
        }

        GLCheckError();
    }

    bool GLWidget::InitTriangleBuffers()
    {
        info("GLWidget: {}", __FUNCTION__);

        // VAO
        glGenVertexArrays(1,&mTriangleVao);
        if (mTriangleVao < 0)
        {
            error("GLWidget: Error creating Triangle VAO");
            return false;
        }
        glBindVertexArray(mTriangleVao);

        // VBO
        glGenBuffers(1,&mTriangleVbo);
        if (mTriangleVbo < 0)
        {
            error("GLWidget: Error creating Triangle VBO");
            return false;
        }
        glBindBuffer(GL_ARRAY_BUFFER,mTriangleVbo);

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
        if (mTriangleVao != -1 && mTriangleVbo != -1)
        {
            info("GLWidget: Triangle VAO/VBO Init Successful");
            return true;
        }
        else
        {
           error("GLWidget: Triangle VAO/VBO Error VAO:{} VBO:{}",mTriangleVao,mTriangleVbo);
        }
        return false;
    }

    bool GLWidget::InitLineBuffers()
    {
        debug("GLWidget: {}", __FUNCTION__);

        // VAO
        glGenVertexArrays(1,&mLineVao);
        if (mLineVao < 0)
        {
            error("GLWidget: Error creating Line VAO");
            return false;
        }
        glBindVertexArray(mLineVao);

        // VBO
        glGenBuffers(1,&mLineVbo);
        if (mLineVbo < 0)
        {
            error("GLWidget: Error creating Line VBO");
            return false;
        }
        glBindBuffer(GL_ARRAY_BUFFER,mLineVbo);

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
        if (mLineVao != -1 && mLineVbo != -1)
        {
            info("GLWidget: Line VAO/VBO Init Successful");
            return true;
        }
        else
        {
           error("GLWidget: Line VAO/VBO Error VAO:{} VBO:{}",mLineVao,mLineVbo);
        }
        return false;
    }

    bool GLWidget::InitPointBuffers()
    {
        info("GLWidget: {}", __FUNCTION__);

        // VAO
        glGenVertexArrays(1,&mPointVao);
        if (mPointVao < 0)
        {
            error("GLWidget: Error creating Point VAO");
            return false;
        }
        glBindVertexArray(mPointVao);

        // VBO
        glGenBuffers(1,&mPointVbo);
        if (mLineVbo < 0)
        {
            error("GLWidget: Error creating Point VBO");
            return false;
        }
        glBindBuffer(GL_ARRAY_BUFFER,mPointVbo);

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
        if (mPointVao != -1 && mPointVbo != -1)
        {
            info("GLWidget: Point VAO/VBO Init Successful");
            return true;
        }
        else
        {
           error("GLWidget: Point VAO/VBO Error VAO:{} VBO:{}",mPointVao,mPointVbo);
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

        // Enable shader program
		glUseProgram(mShaderProgram);
		GLCheckError();

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

        if (!mLineVertexBuffer.empty())
        {
            // Vertex Array
            glBindVertexArray(mLineVao);
        	GLCheckError();

            // Draw
            GLuint sz = mLineVertexBuffer.size();
            debug("GLWidget: Drawing {} lines", sz/2);
            glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(sz));
        	GLCheckError();

            glBindVertexArray(0);
        	GLCheckError();
        }

        if (!mTriangleVertexBuffer.empty())
        {
            // Vertex Array
            glBindVertexArray(mTriangleVao);
        	GLCheckError();

            // Draw
            GLuint sz = mTriangleVertexBuffer.size();
            debug("GLWidget: Drawing {} Triangles", sz/3);
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(sz));
        	GLCheckError();

            glBindVertexArray(0);
        	GLCheckError();
        }

        if (!mPointVertexBuffer.empty())
        {
            // Vertex Array
            glBindVertexArray(mPointVao);
        	GLCheckError();

            // Draw
            GLuint sz = mPointVertexBuffer.size();
            debug("GLWidget: Drawing {} Points", sz);
            glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(sz));
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
        info("GLWidget: {}", __FUNCTION__);
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

    void GLWidget::AddLineVertex(const GLWidgetVertex& lv)
    {
        mLineVertexBuffer.push_back(lv);
    }

    void GLWidget::AddLineVertices(const vector<GLWidgetVertex>& lv)
    {
        mLineVertexBuffer.insert(mLineVertexBuffer.end(),lv.begin(),lv.end());
    }

    void GLWidget::AddTriangleVertex(const GLWidgetVertex& lv)
    {
        mTriangleVertexBuffer.push_back(lv);
    }

    void GLWidget::AddTriangleVertices(const vector<GLWidgetVertex>& lv)
    {
        mTriangleVertexBuffer.insert(mTriangleVertexBuffer.end(), lv.begin(), lv.end());
    }

    void GLWidget::AddPointVertex(const GLWidgetVertex& lv)
    {
        mPointVertexBuffer.push_back(lv);
    }

    void GLWidget::AddPointVertices(const vector<GLWidgetVertex>& lv)
    {
        mPointVertexBuffer.insert(mPointVertexBuffer.end(), lv.begin(), lv.end());
    }


    void GLWidget::ClearLineVertexBuffer()
    {
       mLineVertexBuffer.clear();
    }

	void GLWidget::ClearTriangleVertexBuffer()
    {
       mTriangleVertexBuffer.clear();
    }

    void GLWidget::ClearPointVertexBuffer()
    {
       mPointVertexBuffer.clear();
    }

	void GLWidget::SubmitLineVertexBuffer()
    {
        if (!mLineVertexBuffer.empty())
        {
			// Vertex Array
			glBindVertexArray(mLineVao);
			glBindBuffer(GL_ARRAY_BUFFER, mLineVbo);
			glBufferData(GL_ARRAY_BUFFER,
				static_cast<GLint>(mLineVertexBuffer.size() * sizeof(GLWidgetVertex)),
				&mLineVertexBuffer[0], GL_STATIC_DRAW);
			glBindVertexArray(0);
        }
    }

	void GLWidget::SubmitTriangleVertexBuffer()
    {
        if (!mTriangleVertexBuffer.empty())
        {
			// Vertex Array
			glBindVertexArray(mTriangleVao);
			glBindBuffer(GL_ARRAY_BUFFER, mTriangleVbo);
			glBufferData(GL_ARRAY_BUFFER,
				static_cast<GLint>(mTriangleVertexBuffer.size() * sizeof(GLWidgetVertex)),
				&mTriangleVertexBuffer[0], GL_STATIC_DRAW);
			glBindVertexArray(0);
        }
    }

    void GLWidget::SubmitPointVertexBuffer()
    {
        if (!mPointVertexBuffer.empty())
        {
			// Vertex Array
			glBindVertexArray(mPointVao);
			glBindBuffer(GL_ARRAY_BUFFER, mPointVbo);
			glBufferData(GL_ARRAY_BUFFER,
				static_cast<GLint>(mPointVertexBuffer.size() * sizeof(GLWidgetVertex)),
				&mPointVertexBuffer[0], GL_STATIC_DRAW);
			glBindVertexArray(0);
        }
    }
}
