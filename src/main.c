#include <stdio.h>
#include <math.h>
#include <string.h>

#include "display.h"
#include "filehandling.h"

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
        0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 1.0f, 0.0f, 0.0f
        
    };
    
    GLint elements[] = {
        0, 1, 2,
        0, 2, 3
    };
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), &vert, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), &elements, GL_STATIC_DRAW);
    
    const char* vertexsrc = openFile("../../../shaders/vertex.sdr");
    const char* fragsrc = openFile("../../../shaders/frag.sdr");
    
    GLint status;
    GLchar* log;
    
    GLuint vertexshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexshader, 1, &vertexsrc, NULL);
    
    glCompileShader(vertexshader);
    
    GLuint fragshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragshader, 1, &fragsrc, NULL);
    
    glCompileShader(fragshader);
    
    GLuint prg = glCreateProgram();
    glAttachShader(prg, vertexshader);
    glAttachShader(prg, fragshader);
    
    glBindFragDataLocation(prg, 0, "outColor");
    glLinkProgram(prg);
    glUseProgram(prg);
    
    GLint attrib = glGetAttribLocation(prg, "position");
    glVertexAttribPointer(attrib, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, 0);
    
    GLint color = glGetAttribLocation(prg, "color");
    glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, sizeof(GLfloat) * 2);
    
    glEnableVertexAttribArray(color);
    glEnableVertexAttribArray(attrib);
    
    GLenum error = glGetError();
    printf("%u\n",error);
    
    //    GLint pos = glGetUniformLocation(prg, "pos");
    
    unsigned char count = 0;
    
    while(1) {
        glClear(GL_COLOR_BUFFER_BIT);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        //glUniform3f(unicolor, 0,  (128.0f - fabs(count - 128)) / 128.0f, (128 - fabs(count - 128)) / 128.0f);
        //glUniform2f(pos, (128.0f - fabs(count - 128)) / 128.0f, 1.0f);
        
        count++;
        glfwSwapBuffers(testwin);
        glfwPollEvents();
    }
    
    closeFiles();
}

