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

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    /* TODO: Bind the shader you want to use */
    shader.Bind();
    /* TODO: Bind the VertexArray (vao) and the Index Buffer (ibo) */
    va.Bind();
    ib.Bind();

    // Draw Graphics method (ibo):
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
    // Draw Graphics method (vao):
    // GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}