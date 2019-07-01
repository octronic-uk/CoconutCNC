#pragma once


#include <string>
#include "Common/Logger.h"

#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #include <glad/glad.h>
    #include <OpenGL/gl.h>
#endif

#ifdef __linux__
    #include <glad/glad.h>
    #include <GL/gl.h>
#endif

#ifdef _WIN32
    #ifndef NOMINMAX
        #define NOMINMAX // prevent windows redefining min/max
    #endif

    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif

    #include <windows.h>
    #include <glad/glad.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

/**
* @brief Used to check for OpenGL Runtime Errors. This will display the
* file and line from which the error was detected. This function should
* not be called directly. Instead use the macro GLCheckError.
*
* @return True if an error was detected.
*/

using std::string;

static bool _GLCheckError_(const string& file, int line)
{
    /*
	GLenum errorCode = 0;
	bool wasError = false;
	do
	{
	  errorCode = glad_glGetError();
	  if (errorCode!=0)
	  {
		  error("OpenGL Error Check {}:{} ", file, line);
		  switch (errorCode)
		  {
			  case GL_NO_ERROR:
				  error("\tGL_NO_ERROR");
				  break;
			  case GL_INVALID_ENUM:
				  error("\tGL_INVALID_ENUM" );
				  break;
			  case GL_INVALID_VALUE:
				  error("\tGL_INVALID_VALUE");
				  break;
			  case GL_INVALID_OPERATION:
				  error("\tGL_INVALID_OPERATION");
				  break;
			  #ifndef WIN32
			  case GL_INVALID_FRAMEBUFFER_OPERATION:
				  error("\tGL_INVALID_FRAMEBUFFER_OPERATION");
				  break;
			  #endif
			  case GL_OUT_OF_MEMORY:
				  error("\tGL_OUT_OF_MEMORY");
				  break;
              default:
                  error("\t Unknown Error");
                  break;
		  }
		  error("\tCode: {}" , errorCode );
		  wasError = true;
	  }
	}
	while(errorCode != 0);
	return wasError;
    */
    return false;
}

#define GLCheckError() _GLCheckError_(__FILE__, __LINE__)
