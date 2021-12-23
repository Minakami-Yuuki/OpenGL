#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray() 
{
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    /* VAO (global) is to bind the VertexAttribArray */
    /* So, you can use the vao to link different buffers!! (vao ¡ú vbo) */

    // bind the VertexArray and the VertexBuffer
    // Sequence: Bind vao / vbo / layout / ibo 
    // Eventually: Unbind vao.

    // unsigned int vao;
    // GLCall(glGenVertexArrays(1, &vao));
    // GLCall(glBindVertexArray(vao);

    Bind();
	vb.Bind();

    const auto& elements = layout.GetElements();
    unsigned int offset = 0;

    for (int i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];

        /* You should tell the Attributions to the OpenGL to use them in GPU */
        /* mean: start the first Vertex's Attribution */
        GLCall(glEnableVertexAttribArray(i));
        /* set the Vertex's Attribution
           index: 0                         (the first Attribution)
           size: 2                          (Only float x and float y)
           type: float                      (Attributions' type)
           nomalized: false                 (Now, we dont change it to the Stream)
           stride: sizeof(float) * 2        (it means: How many bytes from the first var to the second var) (float: 4 B/var)
           offset: (const void *)0          (To show the position * sizeof(Byte) of Attribution var)
        */
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
        offset += VertexBufferElement::GetSizeOfType(element.type) * element.count;
    }
}