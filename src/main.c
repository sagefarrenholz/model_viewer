#include <stdio.h>
#include <math.h>
#include <string.h>


#include "display.h"
#include "filehandling.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void loop(GLFWwindow*);

int main() {
    initOpenGL();
    simpleWindowHints();
    setSize(500, 500);
    setTitle("opengl_test");
    startDisplay(&loop);

    return 0;
}

void loop(GLFWwindow* testwin){
    
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    GLuint vbo;
    glGenBuffers(1, &vbo);
    
    GLuint ebo;
    glGenBuffers(1, &ebo);
    
    GLfloat vert[] = {
        0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f
        
    };
    
    GLint elements[] = {
        0, 1, 2,
        0, 2, 3
    };

    //Bind vbo to arraybuffer and buffer verts into it
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), &vert, GL_STATIC_DRAW);
    
    //bind ebo to element buffer and buffer elements into it
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), &elements, GL_STATIC_DRAW);
    
    //opens shaders
    const char* vertexsrc = openFile("../../shaders/vertex.sdr");
    const char* fragsrc = openFile("../../shaders/frag.sdr");
    
    //loads textures using std_image
    GLuint tex;
    int x, y, n;
    stbi__vertically_flip_on_load = 1;
    char* texture = stbi_load("../../textures/pizza.jpg", &x, &y, &n, 0);
    if(texture == NULL){
        printerr(4, "TEXTURE LOAD ERROR");
    }
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glGenerateMipmap(GL_TEXTURE_2D);
    
    //Creates a vertex shader using the shader file
    GLuint vertexshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexshader, 1, &vertexsrc, NULL);
    glCompileShader(vertexshader);
    
    //Compiles a frag shader using the frag source
    GLuint fragshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragshader, 1, &fragsrc, NULL);
    
    glCompileShader(fragshader);
    
    //Clear files in memory
    closeFiles();
    
    //Creates complete shader progam and attaches shaders
    //Also explicitly binds the output of the frag shader to the 0 color buffer
    GLuint prg = glCreateProgram();
    glAttachShader(prg, vertexshader);
    glAttachShader(prg, fragshader);
    
    glBindFragDataLocation(prg, 0, "outColor");
    glLinkProgram(prg);
    glUseProgram(prg);
    
    //
    GLint texatt = glGetAttribLocation(prg, "texcoord");
    glVertexAttribPointer(texatt, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 7, sizeof(GL_FLOAT) * 5);
    
    //Specifies how the vertex shader accesses vertex array data
    GLint attrib = glGetAttribLocation(prg, "position");
    glVertexAttribPointer(attrib, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, 0);
    
    //Specifies how the vertex shader accesses color data in the array
    GLint color = glGetAttribLocation(prg, "color");
    glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, sizeof(GLfloat) * 2);
    
    glEnableVertexAttribArray(texatt);
    glEnableVertexAttribArray(color);
    glEnableVertexAttribArray(attrib);
    
    //Print any errors, 0 if none
    GLenum error = glGetError();
    printf("%u\n",error);
    
    while(1) {
        glClear(GL_COLOR_BUFFER_BIT);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(testwin);
        glfwPollEvents();
    }
    

}

