//
//  sf_window.h
//  opengl-test
//
//  Created by Sage Farrenholz on 1/16/19.
//  Copyright © 2019 Sage Farrenholz. All rights reserved.
//

#ifndef sf_window_h
#define sf_window_h

#include "sfi_gl.h"

#include "sf_renderer.h"

typedef struct _SF_Window {
	GLFWwindow* glfw_window;	// GLFW Window object
	SF_Renderer* renderer;		// Renderer attached to the window
} SF_Window;


// Initializes and returns a glfw window
SF_Window* sf_init_window(int width, int height, const char* title);

// Add a renderer to a window, if NULL a default renderer is created. Rendererer is returned
SF_Renderer* sf_add_renderer(SF_Renderer*);

void sf_destroy_window(SF_Window*);

#endif /* display_h */
