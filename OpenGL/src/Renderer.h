#pragma once

#include <gl/glew.h>

/* define a debugbreak */
// the GLCall() you can use it to show debug Info for whatever's code you want
#define ASSERT(x) if (!(x)) __debugbreak()
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

/* check Error fuctions */
void GLClearError();

/* Judge Error and input the Error Info */
bool GLLogCall(const char* function, const char* file, int line);