#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* TODO: Change the GLFW to the Core_Profile (3.3.0) */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "Welcome to OpenGL!", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    /* Open the rendering */
    glfwMakeContextCurrent(window);

    /* init the GLEW after start Randering (Open makeContext) */
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error!" << std::endl;
    }

    /* You can also print the GLEW_Version */
    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        /* produce the buffer to Draw graphics */
        float positions[] = {
            // Texture: Left-Down is 0,0  ;  Right-Top is 1,1  ;
             -0.5f, -0.5f, 0.0f, 0.0f,   // 0
              0.5f, -0.5f, 1.0f, 0.0f,   // 1
              0.5f,  0.5f, 1.0f, 1.0f,   // 2
             -0.5f,  0.5f, 0.0f, 1.0f    // 3

          // -0.5f, -0.5f, 0.0f, 0.0f,   // 0
          // -0.5f,  0.5f, 0.0f, 1.0f,   // 1
          //  0.0f,  0.0f, 0.5f, 0.5f,   // 2
          //  0.5f, -0.5f, 1.0f, 0.0f,   // 3 
          //  0.5f,  0.5f, 1.0f, 1.0f    // 4
        };

        /* Use index buffer the draw a square (it can reduce GPUs occupancy rate) */
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
          //2, 3, 4
        };

        /* ::MAIN:: */

        // To be continue...
        //GLCall(glEnable(GL_BLEND));
        //GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        /* VertexArray (vao) */
        VertexArray va;

        /* VertexBuffer (vbo) */
        VertexBuffer vb(positions, 4 * 2 * 2 * sizeof(float));

        /* VertexLayout */
        VertexBufferLayout layout;

        /* Bind the vb and layout to the va */
        /* push(number): number means how many Elements in an attribution */
        layout.push<float>(2);
        layout.push<float>(2);
        va.AddBuffer(vb, layout);

        /* Index Buffer Object (ibo) */
        IndexBuffer ib(indices, 6);
         

        /* TODO: Define the VertexShader and the FragmentShader (Fomulation) */
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        /* Use the uniform to set the Fragment shader */
        shader.SetUniform4f("u_Color", 0.8f, 0.2f, 0.8f, 1.0f);
        
        /* Bind the picture that you want to be texture */
        Texture texture("res/textures/Misaki2.png");
        /* Bind the texture attribution */
        // default: slot = 0
        texture.Bind(/* 0 */);
        shader.SetUniform1i("u_Texture", 0);    // 0 == slot

        /* TODO: Unbind everything (init) */
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        /* make Renderer */
        Renderer renderer;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            /* Write main method to compelet Rasterizer */
            /*glBegin(GL_TRIANGLES);
            glVertex2f(-0.5f, -0.5f);
            glVertex2f( 0.0f,  0.5f);
            glVertex2f( 0.5f, -0.5f);
            glEnd();*/

            /* Real - timed update the color of triangle */
            GLfloat timeValue = glfwGetTime();
            GLfloat redValue = (sin(timeValue) / 2) + 0.5;
            GLfloat greenValue = (cos(timeValue) / 2) + 0.5;
            GLfloat blueValue = (sin(timeValue) * cos(timeValue / 2)) + 0.5;
            // Bind the shader to use the Uniform
            shader.Bind();
            shader.SetUniform4f("u_Color", redValue, greenValue, blueValue, 1.0f);

            // Bind the va (vb), ib, shader and Draw the Graphics
            renderer.Draw(va, ib, shader);

            // Draw Graphics by Line or Fill
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
            // GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));

            GLClearError();
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        // Destroy Shader (in ~Shader.cpp)
        // GLCall(glDeleteProgram(shader));
    }
    glfwTerminate();
    return 0;
}