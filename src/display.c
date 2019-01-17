//
//  display.c
//  opengl-test
//
//  Created by Sage Farrenholz on 1/16/19.
//  Copyright © 2019 Sage Farrenholz. All rights reserved.
//

#include "display.h"
#include "errorhandling.h"

static unsigned width, height;
static char* title;
static GLFWwindow* window;

void initOpenGL(void) {
    assert(!glfwInit());
}

void simpleWindowHints(void) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

int setSize (const unsigned inwidth, const unsigned inheight) {
    width = inwidth;
    height = inheight;
    return 0;
}

int setTitle (const char* intitle) {
    title = intitle;
    return 0;
}

int startDisplay(void (*gameloop)(GLFWwindow*)){
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    glfwMakeContextCurrent(window);
    gameloop(window);
    glfwTerminate();
    return 0;
}
