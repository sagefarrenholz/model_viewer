
#include <stdlib.h>
#include <string.h>

#include "sf_renderer.h"
#include "sf_error.h"

#include "util/memory.h"
#include "util/filehandling.h"

SF_Renderer* sf_init_renderer(void) {
	SF_Renderer* renderer = (SF_Renderer*) mem_malloc(sizeof(SF_Renderer));

	if (renderer == NULL) {
		// TODO return error code or set some error bit
		sf_print_error(SF_UNSPECIFIED_ERROR, "Could not allocate memory for SF_Renderer");
		return NULL;
	}

	GLuint _vao = -1;
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	
	renderer->vao = _vao;
	renderer->program = -1;
}

void sf_init_program(SF_Renderer* ren) {
	ren->program = glCreateProgram();
}

void sf_add_shader(SF_Renderer* ren, const char* shader_src_path, GLenum shader_type) {

	if (ren == NULL) {
		sf_print_error(SF_UNSPECIFIED_ERROR, "SF_Renderer* renderer may not be null in call to sf_add_shader");
		return;
	}

	if (shader_src_path == NULL) {
		sf_print_error(SF_UNSPECIFIED_ERROR, "const char* shader_src_path may not be null in call to sf_add_shader");
		return;
	}
	const char* shader_src = readFile(shader_src_path);
	if (shader_src_path == NULL) {
		sf_print_error(SF_UNSPECIFIED_ERROR, "Failed to read file in sf_add_shader");
		return;
	}

	GLuint shader_ref = glCreateShader(shader_type);
	glShaderSource(shader_ref, 1, &shader_src, NULL);
	glCompileShader(shader_ref);
	if (ren->program == -1) {
		sf_init_program(ren);
	}
	glAttachShader(ren->program, shader_ref);

	switch (shader_type) {
		case GL_VERTEX_SHADER:
			ren->vertex_shader = shader_ref;
			break;
		case GL_FRAGMENT_SHADER:
			ren->frag_shader = shader_ref;
			break;
	}
	
}

void sf_finalize_shaders(SF_Renderer* ren) {
	glLinkProgram(ren->program);
	glUseProgram(ren->program);
}

void sf_destroy_renderer(SF_Renderer* ren) {
	glDeleteShader(ren->vertex_shader);
	glDeleteShader(ren->frag_shader);
	glDeleteProgram(ren->program);
	mem_free(ren);
}