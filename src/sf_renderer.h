#ifndef sf_renderer_h
#define sf_renderer_h

#include "sfi_gl.h"

#include <stdbool.h>

typedef struct _SF_Renderer {

	GLuint vertex_shader;
	char* vertex_src;
	GLuint frag_shader;
	char* frag_src;

	GLuint program;

	GLuint vao;
	GLuint* vbo;

	//SF_Object* objects;

} SF_Renderer;


// Initialize a renderer
SF_Renderer* sf_init_renderer(void);

// Draws a frame
void sf_draw(SF_Renderer*);

// Load a shader into the current context
void sf_add_shader(SF_Renderer* renderer, const char* shader, GLenum shader_type);

// Finalizes all of the attached shaders and commits them to the renderer.
void sf_finalize_shaders(SF_Renderer* renderer);

// Destroys the given renderer
void sf_destroy_renderer(SF_Renderer* renderer);

// Load a texture
GLint sf_load_texture(const char* filename);

#endif sf_renderer_h