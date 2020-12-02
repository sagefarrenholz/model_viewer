#ifndef sfi_gl_h
#define sfi_gl_h

#define GLEW_STATIC

//Includes for Windows
#ifdef _WIN32
#include <glew.h>
#include <glfw3.h>
#endif // WINDOWS

//Includes for Mac
#ifdef MAC
#include <OpenGL/gl3.h>
#endif // MAC

#ifdef LINUX
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#endif