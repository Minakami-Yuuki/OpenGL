#include "Renderer.h"

#include <iostream>

/* check Error fuctions */
void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

/* Judge Error and input the Error Info */
bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    }

    return true;
}