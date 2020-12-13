//
//  display.c
//  opengl-test
//
//  Created by Sage Farrenholz on 1/16/19.
//  Copyright © 2019 Sage Farrenholz. All rights reserved.
//

#include "sf_window.h"
#include "sf_error.h"

#include "util/memory.h"

SF_Window* sf_init_window(int width, int height, const char* title) {

	SF_Window* win = (SF_Window*) mem_malloc(sizeof(SF_Window));

	if (win == NULL) {
		sf_print_error(SF_UNSPECIFIED_ERROR, "Could not allocate memory SF_Window in sf_init_window");
		return NULL;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	glfwSetErrorCallback(sf_print_error);
	if (!glfwInit()) {
		sf_print_error(SF_UNSPECIFIED_ERROR, "Could not intialize GLFW.");
		return NULL;
	}

    if (width <= 0) width = 1920;
    if (height <= 0) height = 1080;
    if (title == NULL) title = "Super Fast Engine";

    GLFWwindow* _glfw_window = glfwCreateWindow(width, height, title, /*glfwGetPrimaryMonitor()*/ NULL, NULL);
	if (_glfw_window == NULL) {
		sf_print_error(SF_UNSPECIFIED_ERROR, "Could not create glfw window in sf_init_window");
		sf_destroy_window(win);
		return NULL;
	}
    glfwMakeContextCurrent(_glfw_window);

	{
		glewExperimental = GL_TRUE;
		GLenum glew_err = glewInit();
		if (glew_err != GLEW_OK) {
			sf_print_debug("Glew error.");
			sf_print_error(SF_UNSPECIFIED_ERROR, glewGetErrorString(glew_err));
			return NULL;
		}
	}

	win->glfw_window = _glfw_window;

	return win;
}

void sf_destroy_window(SF_Window* win) {
	glfwTerminate();
	mem_free(win);    
}