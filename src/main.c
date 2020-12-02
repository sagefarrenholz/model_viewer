/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  0.1
 *        Created:  06/06/2020 04:56:19 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sage Farrenholz (C) 2020, 
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "sf.h"

#include "util/memory.h"
#include "util/filehandling.h"
#include "util/interleave.h"

#define TINYOBJ_LOADER_C_IMPLEMENTATION
#include <tinyobj_loader_c.h>

#define TINYOBJ_CALLOC mem_calloc
#define TINYOBJ_MALLOC mem_malloc
#define TINYOBJ_MALLOC mem_free

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void* key(GLFWwindow*, int, int, int, int);
void loop(GLFWwindow*);

static float angley = 0;
static float anglex = 0;

static float dist = 0;

static int8_t togglex = 0, toggley = 0, togglem = 0;

void camera_input(GLFWwindow*, double, double);

int main() {
	sf_init();

	SF_Window* window = sf_init_window(0, 0, NULL);
	glfwSetKeyCallback(window->glfw_window, key);

	while (!glfwWindowShouldClose(window->glfw_window)) {
		glfwPollEvents();
	}

	sf_quit();
	return 0;
}

void parseObjFile(const char* name, int is_material , const char* name_again, char** data, unsigned __int64* len) {
	*data = readFile(name);
	*len = strlen(*data);
}

/*
void loop(){

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    GLuint vbo;
    glGenBuffers(1, &vbo);
    
    GLuint ebo;
    glGenBuffers(1, &ebo);

	tinyobj_attrib_t attribm;
	tinyobj_shape_t* shape = NULL;
	tinyobj_material_t* materials = NULL;

	size_t shape_num;
	size_t material_num;

	tinyobj_attrib_init(&attribm);
	const char* model_path = "models/cube.obj";
	printf("Model path: %s\n", model_path);
	//char* model3d = readFile(model_path);
	
	int result = tinyobj_parse_obj(&attribm, &shape, &shape_num, &materials, &material_num, model_path, parseObjFile, 0);

	printf("Model load error: %i\n", result);

	//Buffer for storing face values
	int* face_buff = (int*) mem_calloc(attribm.num_faces, sizeof(int));

	//Fill it with face values from obj loader
	for (unsigned i = 0; i < attribm.num_faces; i++)
		face_buff[i] = attribm.faces[i].v_idx;

	//Need these buffers to expand the vertex data for formatting and interleaving
	float* texbuff = (float*) mem_calloc(attribm.num_faces * 2, sizeof(float));
	float* verbuff = (float*) mem_calloc(attribm.num_faces * 3, sizeof(float));

	for (unsigned i = 0; i < attribm.num_faces; i++) {
		for (int c = 0; c < 2; c++) {
			texbuff[2*i+c] = attribm.texcoords[2 * (attribm.faces[i].vt_idx) + c];
		}
	}

	for (unsigned i = 0; i < attribm.num_faces; i++) {
		for (int c = 0; c < 3; c++) {
			verbuff[3*i + c] = attribm.vertices[3 * (attribm.faces[i].v_idx) + c];
		}
	}

	float* arraybuff = interleave(verbuff, texbuff, attribm.num_faces * 5);

    //Bind vbo to arraybuffer and buffer verts into it
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * attribm.num_faces * 5, arraybuff, GL_STATIC_DRAW);
   
    //bind ebo to element buffer and buffer elements into it
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * attribm.num_faces, face_buff, GL_STATIC_DRAW);

    //opens shaders
    const char* vertexsrc = readFile("shaders/vertex.vert");
    const char* fragsrc = readFile("shaders/frag.frag");
    
    //loads textures using std_image
    GLuint tex;
    int x, y, n;
    stbi__vertically_flip_on_load = 1;
    char* texture = stbi_load("textures/dirt.png", &x, &y, &n, 0);
    if(texture == NULL)
        printerr(4, "TEXTURE LOAD ERROR");

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
	stbi_image_free(texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glGenerateMipmap(GL_TEXTURE_2D);

    //Creates a vertex shader using the shader file
    GLuint vertexshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexshader, 1, &vertexsrc, NULL);
    glCompileShader(vertexshader);

	//glfwSetKeyCallback(testwin,key);

    //Compiles a frag shader using the frag source
    GLuint fragshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragshader, 1, &fragsrc, NULL);
    
    glCompileShader(fragshader);

    //Creates complete shader progam and attaches shaders
    //Also explicitly binds the output of the frag shader to the 0 color buffer
    GLuint prg = glCreateProgram();
    glAttachShader(prg, vertexshader);
    glAttachShader(prg, fragshader);

	GLint fragcompile, vercompile;
	glGetShaderiv(fragshader, GL_COMPILE_STATUS, &fragcompile);
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &vercompile);
	
    glBindFragDataLocation(prg, 0, "outColor");

    glLinkProgram(prg);
    glUseProgram(prg);

	#define LOG 1024

	char* temp = (char*) mem_calloc(LOG, sizeof(char));
	char* temp2 = (char*) mem_calloc(LOG, sizeof(char));

	//Logs shader compilation info
	glGetShaderInfoLog(vertexshader, LOG, NULL,temp);

	writeFile("log/shaders/vertex.log", temp);

	glGetShaderInfoLog(fragshader, LOG, NULL, temp2);

	writeFile("log/shaders/frag.log", temp2);

	GLint thetax = glGetUniformLocation(prg, "thetax");
	GLint thetay = glGetUniformLocation(prg, "thetay");
	GLint trans = glGetUniformLocation(prg, "move");

	glfwSetCursorPosCallback(testwin, camera_input);
	glfwSetInputMode(testwin, GLFW_CURSOR,GLFW_CURSOR_DISABLED);

    //Binds the variables used in the shader program to texture coordinates
    GLint texatt = glGetAttribLocation(prg, "intexcoord");
    glVertexAttribPointer(texatt, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, sizeof(GL_FLOAT) * 3);

    //Specifies how the vertex shader accesses vertex array data
    GLint attrib = glGetAttribLocation(prg, "position");
    glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, 0);

    glEnableVertexAttribArray(texatt);
    //glEnableVertexAttribArray(color);
    glEnableVertexAttribArray(attrib);

	time_t lasttime = time(NULL);
	time_t currtime = time(NULL);
	unsigned frmcnt = 0;

	GLenum err = glGetError();
	printf("GL_ERROR: %u\nVertex compile? %i\nFrag compile? %i\n", err, vercompile, fragcompile);

	glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(testwin)) {
		time(&currtime);
		frmcnt++;

		if (currtime - lasttime >= 1) {
			printf("%u fps\n", frmcnt);
			frmcnt = 0;
			lasttime = currtime;
		}
		frmcnt++;
		if (togglex) anglex+=5 * togglex;
		if (toggley) angley+=5 * toggley;
		if (togglem) dist += 2 * togglem;
		glUniform1f(thetax, anglex * (2.0f * 3.14f / 510.0f));
		glUniform1f(thetay, angley * (2.0f * 3.14f / 510.0f));
		glUniform1f(trans, 2.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, attribm.num_faces * 5);
        glfwSwapBuffers(testwin);
        glfwPollEvents();
    }

	tinyobj_attrib_free(&attribm);
	tinyobj_shapes_free(shape, shape_num);
	tinyobj_materials_free(materials, material_num);

}*/

void* key(GLFWwindow* win, int key, int scancode, int action, int mods) {
	if(action == GLFW_PRESS)
		switch (key) {
			case GLFW_KEY_UP:
				togglex = 1;
				break;
			case GLFW_KEY_DOWN:
				togglex = -1;
				break;
			case GLFW_KEY_RIGHT:
				toggley = 1;
				break;
			case GLFW_KEY_LEFT:
				toggley = -1;
				break;
			case GLFW_KEY_W:
				togglem = 1;
				break;
			case GLFW_KEY_S:
				togglem = -1;
				break;
			case GLFW_KEY_ESCAPE:
				glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		}
	else if(action == GLFW_RELEASE)
		switch (key) {
			case GLFW_KEY_UP:
				togglex = 0;
				break;
			case GLFW_KEY_DOWN:
				togglex = 0;
				break;
			case GLFW_KEY_RIGHT:
				toggley = 0;
				break;
			case GLFW_KEY_LEFT:
				toggley = 0;
				break;
			case GLFW_KEY_W:
				togglem = 0;
				break;
			case GLFW_KEY_S:
				togglem = 0;
				break;
			case GLFW_KEY_ESCAPE:
				glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	return 0;
}

// mouse input
void camera_input(GLFWwindow* win, double xpos, double ypos) {
	angley = -1 * xpos; anglex = -1* ypos;
}

