#pragma once

#include<GL/glew.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError;\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

/* This function clears openGL error flags, but does not print them. */
void GLClearError();

/* This function clears the openGL error flags and prints them as it goes. */
bool GLLogCall(const char* function, const char* file, int line);