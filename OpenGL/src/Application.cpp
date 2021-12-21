#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

/* define a debugbreak */
// the GLCall() you can use it to show debug Info for whatever's code you want
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__));

/* check Error fuctions */
static void GLClearError() 
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function << "\n" << file << ":" << line << std::endl;
        return false;
    }

    return true;
}

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

// In order to reduce the complicated and replaced Shader creating
static ShaderProgramSource ParseShader(const std::string& filepath) 
{
    // Input the filepath
    std::ifstream stream(filepath);

    // choose Vertex or Fragment
    enum class ShaderType
    {
        NONE        = -1,
        VERTEX      = 0,
        FRAGMENT    = 1
    };

    // ss[] is to distinguish the string::Vertex and the string::Fragment
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }

        else if (type != ShaderType::NONE)
        {
            // Not the #shader vertex or #shader fragment
            // Input the fileline + '\n' directly.
            ss[(int)type] << line << '\n';
        }
    }

    // Eventually retrun the contant of file
    return { ss[0].str(), ss[1].str() };

}

/* TODO: create the Shader Complier (link the shader) */
static unsigned int ComplieShader(unsigned int type, const std::string& source)
{
    // Create the Shader
    unsigned int id = glCreateShader(type);

    // Get the first address of Source 
    // source.c_str() == source[0]
    const char* src = source.c_str();

    // Get the shaderSource (Once per compile)
    glShaderSource(id, 1, &src, nullptr);

    // Compile:
    glCompileShader(id);


    // TODO: Error handing.
    // result is to save the address of Shaderiv
    int result;

    // glGetShaderiv() (make the mistakes(Integer) output to the &result)
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) 
    {
        // save Info
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        // show Info
        // To create a varity Stack:
        char* message = (char*)malloc(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        // Cout the mistake messages
        std::cout << "Fail to Compile " << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << std::endl;
        std::cout << message << std::endl;

        // Dont forget to delete the ERROR shader:
        glDeleteShader(id);

        return 0;
    }


    return id;
}

/* TODO: create the Vertex Shader and Fragment Shader */
static unsigned int CreateShader(const std::string& VertexShader, const std::string& FragmentShader)
{
    // Create the start program and complie the V and F shader
    unsigned int program = glCreateProgram();
    unsigned int vs = ComplieShader(GL_VERTEX_SHADER, VertexShader);
    unsigned int fs = ComplieShader(GL_FRAGMENT_SHADER, FragmentShader);

    // Get attach the program and the Shader:
    // By the way: Link and ValidateLink (By program)
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Welcome to OpenGL!", NULL, NULL);
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

    /* produce the buffer to Draw graphics */
    float positions[] = {
        -0.5f, -0.5f,   // 0
         0.5f, -0.5f,   // 1
         0.5f,  0.5f,   // 2
     //  0.5f,  0.5f,
        -0.5f,  0.5f    // 3
     // -0.5f, -0.5f 
    };

    /* Use index buffer the draw a square (it can reduce GPUs occupancy rate) */
    unsigned int indices[] = {
        0, 1, 2,

        2, 3, 0
    };

    unsigned int buffer;
    GLCall(glGenBuffers(1, &buffer));
    /* use the function to start glDrawArrays() */
    /* use this to choose ID for what you want to Draw */
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    /* You should tell the Attributions to the OpenGL to use them in GPU */
    /* mean: start the first Vertex's Attribution */
    GLCall(glEnableVertexAttribArray(0));
    /* set the Vertex's Attribution 
       index: 0                         (the first Attribution)
       size: 2                          (Only float x and float y)
       type: float                      (Attributions' type)
       nomalized: false                 (Now, we dont change it to the Stream)
       stride: sizeof(float) * 2        (it means: How many bytes from the first var to the second var) (float: 4 B/var)
       pointer: (const void *)0         (To show the position * sizeof(Byte) of Attribution var)
    */
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void *)0));

    /* ibo: index buffer object */
    unsigned int ibo;
    GLCall(glGenBuffers(1, &ibo));
    /* use the function to start glDrawArrays() */
    /* use this to choose ID for what you want to Draw */
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));


    /* TODO: Define the VertexShader and the FragmentShader (Fomulation) */
    /* Start the program */

    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
    std::cout << '\n' << "Vertex Info:" << std::endl;
    std::cout << source.VertexSource << std::endl;
    std::cout << "Fragment Info:" << std::endl;
    std::cout << source.FragmentSource << std::endl;

    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);



    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Write main method to compelet Rasterizer */
        /*glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f( 0.0f,  0.5f);
        glVertex2f( 0.5f, -0.5f);
        glEnd();*/

        // Index buffer
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        // 
        // glDrawArrays(GL_TRIANGLES, 0, 6);

        GLClearError();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // destroy shader
    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}