//
//  display.h
//  opengl-test
//
//  Created by Sage Farrenholz on 1/16/19.
//  Copyright © 2019 Sage Farrenholz. All rights reserved.
//

#ifndef display_h
#define display_h

#include "GLFW/glfw3.h"
#include <OpenGL/gl3.h>

void initOpenGL(void);
void simpleWindowHints(void);
int setSize(unsigned, unsigned);
int setTitle(const char*);
int startDisplay(void (*loop)(GLFWwindow*));

#endif /* display_h */
