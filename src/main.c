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
#include <stdbool.h>

#include "sf.h"

#include "util/memory.h"
#include "util/filehandling.h"
#include "util/interleave.h"

#include <cglm/cglm.h>

#define TINYOBJ_LOADER_C_IMPLEMENTATION
#include <tinyobj_loader_c.h>

#define TINYOBJ_CALLOC mem_calloc
#define TINYOBJ_MALLOC mem_malloc
#define TINYOBJ_MALLOC mem_free

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void* key(GLFWwindow*, int, int, int, int);
void loop();

static float pitch = 45;
static float yaw = 0;

static float last_x = 0;
static float last_y = 0;

static float dist = 0;

static float togglex = 0,
toggley = 0,
toggle_w = 0;
toggle_a = 0;
toggle_s = 0;
toggle_d = 0;
toggle_space = 0;
toggle_control = 0;

static float speed_scalar = 1.0;
static float forward_velocity = 0;
static float lateral_velocity = 0;
static float vertical_velocity = 0;
static int poly_mode = 0;
static vec3 cam = { 4, 4, 4 };
static float camera_zoom_pos[3] = { 0, 0, 0 };


bool rotation = 1;

void camera_input(GLFWwindow*, double, double);

int main() {
	/*sf_init();

	SF_Window* window = sf_init_window(0, 0, NULL);
	glfwSetKeyCallback(window->glfw_window, key);

	while (!glfwWindowShouldClose(window->glfw_window)) {
		glfwPollEvents();
	}

	sf_quit();*/
	loop();
	return 0;
}

void parseObjFile(const char* name, int is_material , const char* name_again, char** data, unsigned __int64* len) {
	*data = readFile(name); 
	SF_DEBUG("parseObjFile %s: ", name);
	*len = strlen(*data);
}


void loop(){

	GLFWwindow* testwin = sf_init_window(0, 0, NULL)->glfw_window;
	glfwSetKeyCallback(testwin, key);

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
	SF_DEBUG("Model path: %s\n", model_path);
	//char* model3d = readFile(model_path);
	
	int result = tinyobj_parse_obj(&attribm, &shape, &shape_num, &materials, &material_num, model_path, parseObjFile, TINYOBJ_FLAG_TRIANGULATE);

	closeFiles();

	assert(attribm.num_faces % 3 == 0);

	/*for (int i = 0; i < attribm.num_faces; i++) {
		//printf("%d ", attribm.faces[i].v_idx);
		if (i % 3 == 0) {
			printf("\n");
		}

	}*/

	//printf("Model load error: %i\n", result);

	//Buffer for storing face values
	//int* face_buff = (int*) malloc(attribm.num_faces * sizeof(int) * 3);

	//printf("num faces %u\n", attribm.num_faces * 3);
	//printf("num faces %u\n", attribm.num_vertices);

	float* arraybuff = mem_malloc(sizeof(float) * attribm.num_faces * 8);

	for (unsigned i = 0; i < attribm.num_faces; i++) {
		arraybuff[i * 8 + 0] = attribm.vertices[3 * (attribm.faces[i].v_idx) + 0];
		arraybuff[i * 8 + 1] = attribm.vertices[3 * (attribm.faces[i].v_idx) + 1];
		arraybuff[i * 8 + 2] = attribm.vertices[3 * (attribm.faces[i].v_idx) + 2];
		arraybuff[i * 8 + 3] = attribm.texcoords[2 * (attribm.faces[i].vt_idx) + 0];
		arraybuff[i * 8 + 4] = attribm.texcoords[2 * (attribm.faces[i].vt_idx) + 1];
		if (attribm.num_normals > 0) {
			arraybuff[i * 8 + 5] = attribm.normals[3 * (attribm.faces[i].vn_idx) + 0];
			arraybuff[i * 8 + 6] = attribm.normals[3 * (attribm.faces[i].vn_idx) + 1];
			arraybuff[i * 8 + 7] = attribm.normals[3 * (attribm.faces[i].vn_idx) + 2];
		}
	}
	tinyobj_attrib_free(&attribm);
	tinyobj_shapes_free(shape, shape_num);
	tinyobj_materials_free(materials, material_num);

    //Bind vbo to arraybuffer and buffer verts into it
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

	GLenum err = glGetError();
	SF_DEBUG("GL_ERROR: %u\n", err);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * attribm.num_faces * 8, arraybuff, GL_STATIC_DRAW);
	mem_free(arraybuff);

	err = glGetError();
	SF_DEBUG("GL_ERROR: %u\n", err);
   
    //bind ebo to element buffer and buffer elements into it

	err = glGetError();
	SF_DEBUG("GL_ERROR: %u\n", err);

    //opens shaders
    const char* vertexsrc = readFile("shaders/vertex.vert");
    const char* fragsrc = readFile("shaders/frag.frag");
    
    //loads textures using std_image
    GLuint tex;
    int x, y, n;
    stbi__vertically_flip_on_load = 1;
    char* texture = stbi_load("textures/dirt.png", &x, &y, &n, 4);
    if(texture == NULL)
        printerr(4, "TEXTURE LOAD ERROR");

    glGenTextures(1, &tex);
	err = glGetError();
	SF_DEBUG("GL_ERROR: %u\n", err);
    glBindTexture(GL_TEXTURE_2D, tex);
	err = glGetError();
	SF_DEBUG("GL_ERROR: %u\n", err);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
	stbi_image_free(texture);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glGenerateMipmap(GL_TEXTURE_2D);

    //Creates a vertex shader using the shader file
    GLuint vertexshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexshader, 1, &vertexsrc, NULL);
    glCompileShader(vertexshader);

	
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
	glBindAttribLocation(prg, 2, "texcoord");

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
	GLint mvp = glGetUniformLocation(prg, "mvp");

	glfwSetCursorPosCallback(testwin, camera_input);
	glfwSetInputMode(testwin, GLFW_CURSOR,GLFW_CURSOR_DISABLED);

    //Binds the variables used in the shader program to texture coordinates

	GLint texatt = glGetAttribLocation(prg, "intexcoord");
	glVertexAttribPointer(texatt, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));

	err = glGetError();
	SF_DEBUG("GL_ERROR: vert %u\n", err);

    //Specifies how the vertex shader accesses vertex array data
    GLint attrib = glGetAttribLocation(prg, "position");
    glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
	
	err = glGetError();
	SF_DEBUG("GL_ERROR: %u\n", err);

    glEnableVertexAttribArray(texatt);

    //glEnableVertexAttribArray(color);
    glEnableVertexAttribArray(attrib);

	time_t lasttime = time(NULL);
	time_t currtime = time(NULL);
	unsigned frmcnt = 0;

	err = glGetError();
	SF_DEBUG("GL_ERROR: %u\nVertex compile? %i\nFrag compile? %i\n", err, vercompile, fragcompile);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	mat4* model = (mat4*)mem_malloc(sizeof(mat4));
	mat4* view = (mat4*) mem_malloc(sizeof(mat4));
	mat4* perp = (mat4*)mem_malloc(sizeof(mat4));
	mat4* view_model = (mat4*)mem_malloc(sizeof(mat4));
	mat4* transform = (mat4*)mem_malloc(sizeof(mat4));

	glm_mat4_identity(model);
	glm_mat4_identity(view);
	//glm_mat4_identity(perp);

	//glm_perspective_default(16.0/9.0,perp);
	glm_perspective(0.9f, 16.0/9.0, 0.1f, 100.0f, perp);
 
	vec3 origin = { 0, 0, 0 };
	vec3 view_dir = { 0, 0, 0 };
	vec3 up_dir = { 0, 0, 0 };
	vec3 z_normal = { 0, 1, 0 };
	vec3 camera_side = { 0, 0 , 0 };
	vec3 at = { 0, 0, 0 };
	glm_vec3_sub(origin, cam, view_dir);
	glm_normalize(view_dir);

	glm_vec3_crossn(z_normal, view_dir, camera_side);
	glm_vec3_crossn(camera_side, view_dir, up_dir);
	glm_vec3_flipsign(up_dir);

	vec3 lateral_movement = {0, 0, 0};
	vec3 vertical_movement = { 0, 0, 0 };
	vec3 forward_movement = { 0, 0, 0 };

	closeFiles();

    while(!glfwWindowShouldClose(testwin)) {
		
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		time(&currtime);
		frmcnt++;

		if (currtime - lasttime >= 1) {
			SF_DEBUG("%u fps\n", frmcnt);
			frmcnt = 0;
			lasttime = currtime;
		}
		//printf("togglef %f\n", togglef);
		frmcnt++;
		//if (togglex) anglex+=5 * togglex;
		//if (toggley) angley+=5 * toggley;
		//if (togglem) dist += 2 * togglem;
		/*glUniform1f(thetax, anglex * (2.0f * 3.14f / 510.0f));
		glUniform1f(thetay, angley * (2.0f * 3.14f / 510.0f));*/

		if (poly_mode == 0) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else if (poly_mode == 1) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else if (poly_mode == 2) glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

		view_dir[0] = 0;
		view_dir[1] = 0;
		view_dir[2] = 1;

		up_dir[0] = 0;
		up_dir[1] = 1;
		up_dir[2] = 0;

		// Camera rotation 
		glm_vec3_rotate(view_dir, glm_rad(-yaw), up_dir);
		glm_vec3_crossn(z_normal, view_dir, camera_side);
		glm_vec3_rotate(view_dir, glm_rad(-pitch), camera_side);
		glm_vec3_normalize(view_dir);
		
		glm_vec3_crossn(camera_side, view_dir, up_dir);
		glm_vec3_flipsign(up_dir);

		glm_vec3_scale(view_dir, speed_scalar * forward_velocity, forward_movement);

		glm_vec3_scale(up_dir, speed_scalar * vertical_velocity * 0.5, vertical_movement);
		glm_vec3_scale(camera_side, speed_scalar * lateral_velocity * 0.5, lateral_movement);
		glm_vec3_add(forward_movement, vertical_movement, forward_movement);
		glm_vec3_add(forward_movement, lateral_movement, forward_movement);

		glm_vec3_add(cam, forward_movement, cam);

		// Camera direction
		glm_vec3_add(cam, view_dir, at);

		// View matrix
		glm_lookat(cam, at, z_normal, view);

		glm_mat4_mul(view, model, view_model);



		// Final transform matrix
		glm_mat4_mul(perp, view_model, transform);

		glUniformMatrix4fv(mvp, 1, GL_FALSE, transform);
		
		glDrawArrays(GL_TRIANGLES, 0, attribm.num_faces * 8);
		//glDrawElements(GL_TRIANGLES, attribm.num_faces, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(testwin);
        glfwPollEvents();
		
    }

}


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
				toggle_w = 1.0;
				forward_velocity = 0.1;
				break;
			case GLFW_KEY_S:
				toggle_s = 1.0;
				forward_velocity = -0.1;
				break;
			case GLFW_KEY_A:
				toggle_a = 1.0;
				lateral_velocity = 0.1;
				break;
			case GLFW_KEY_D:
				toggle_d = 1.0;
				lateral_velocity = -0.1;
				break;
			case GLFW_KEY_SPACE:
				toggle_space = 1.0;
				vertical_velocity = 0.1;
				break;
			case GLFW_KEY_LEFT_CONTROL:
				toggle_control = 1.0;
				vertical_velocity = -0.1;
				break;
			case GLFW_KEY_LEFT_SHIFT:
				speed_scalar =  0.5;
				break;
			case GLFW_KEY_ESCAPE:
				if (glfwGetInputMode(win, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
					glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					rotation = 0;
				}
				else {
					glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
					rotation = 1;
				}	
				break;
			case GLFW_KEY_DELETE:
				cam[0] = 4;
				cam[1] = 4;
				cam[2] = 4;
				break;
			case GLFW_KEY_ENTER:
				poly_mode += 1;
				if (poly_mode == 3) poly_mode = 0;
				SF_DEBUG("poly_mode %d\n", poly_mode);
				break;

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
				toggle_w = 0;
				forward_velocity = 0;
				if (toggle_s > 0) forward_velocity = -0.1;
				break;
			case GLFW_KEY_S:
				toggle_s = 0;
				forward_velocity = 0;
				if (toggle_w > 0) forward_velocity = 0.1;
				break;
			case GLFW_KEY_A:
				toggle_a = 0;
				lateral_velocity = 0;
				if (toggle_d > 0) lateral_velocity = -0.1;
				break;
			case GLFW_KEY_D:
				toggle_d = 0;
				lateral_velocity = 0;
				if (toggle_a > 0) lateral_velocity = 0.1;
				break;
			case GLFW_KEY_SPACE:
				toggle_space = 0;
				vertical_velocity = 0;
				if (toggle_control > 0) vertical_velocity = -0.1;
				break;
			case GLFW_KEY_LEFT_CONTROL:
				toggle_control = 0;
				vertical_velocity = 0;
				if (toggle_space > 0) vertical_velocity = 0.1;
				break;
			case GLFW_KEY_LEFT_SHIFT:
				speed_scalar = 1.0;
				break;
		}
	return 0;
}

// mouse input
void camera_input(GLFWwindow* win, double xpos, double ypos) {
	if (last_y == 0) last_y = ypos;
	if (last_x == 0) last_x = xpos;

	float y_offset = last_y - ypos;
	float x_offset = xpos - last_x;
	last_x = xpos;
	last_y = ypos;

	const double SENSITIVITY = 0.08;
	if (rotation == 1) {
		pitch += y_offset * SENSITIVITY;
		yaw += x_offset * SENSITIVITY;
	}

	if (pitch > 89.0) pitch = 89.0;
	if (pitch < -89.0) pitch = -89.0;
}

