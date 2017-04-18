//
//  MyTriangle.cpp
//  Triangle
//
//  Created by coderwj on 16/10/15.
//  Copyright © 2016年 coderwj. All rights reserved.
//

#include "glew.h"
#include "glfw3.h"
#include <iostream>

#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>


#include "gamescene.h"

GameScene * GameScene::gs = 0;

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

// helper to check and display for shader compiler errors
bool check_shader_compile_status(GLuint obj) {
    GLint status;
    glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE) {
        GLint length;
        glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(length);
        glGetShaderInfoLog(obj, length, &length, &log[0]);
        std::cerr << &log[0];
        return false;
    }
    return true;
}

// helper to check and display for shader linker error
bool check_program_link_status(GLuint obj) {
    GLint status;
    glGetProgramiv(obj, GL_LINK_STATUS, &status);
    if(status == GL_FALSE) {
        GLint length;
        glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(length);
        glGetProgramInfoLog(obj, length, &length, &log[0]);
        std::cerr << &log[0];
        return false;
    }
    return true;
}

GLuint LoadShaders(GLFWwindow *window, const char * vertex_file_path, const char * fragment_file_path){

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    FILE * fp = fopen(vertex_file_path, "r");
    if(fp != NULL)
    {
        char c = fgetc(fp);
        while(c != EOF)
        {
            if(c == '\n')
            {
                VertexShaderCode += "\\n";
            }
            else
            {
                VertexShaderCode += c;
            }
        }
        fclose(fp);
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::string Line = "";
        while(getline(FragmentShaderStream, Line))
            FragmentShaderCode += Line + "\n";
        FragmentShaderStream.close();
    }

    // Compile Vertex Shader
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Compile Fragment Shader
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    if(!check_shader_compile_status(VertexShaderID) || !check_shader_compile_status(FragmentShaderID)) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }


    // Link the program
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    if(!check_program_link_status(ProgramID)) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}

bool GameScene::init(){
    int width = 640;
    int height = 480;


    if(glfwInit() == GL_FALSE) {
        std::cerr << "failed to init GLFW" << std::endl;
        return false;
    }

    glfwSetErrorCallback(error_callback);

    // select opengl version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create a window
    GLFWwindow *window;
    if((window = glfwCreateWindow(width, height, "01shader_vbo1", 0, 0)) == 0) {
        std::cerr << "failed to open window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK) {
        std::cerr << "failed to init GLEW" << std::endl;
        return false;
    }

    std::string vs_path = "/Users/coderwj/myGame/myEngine/res/shader/common.vs";
    std::string ps_path = "/Users/coderwj/myGame/myEngine/res/shader/common.ps";

    // program and shader handles
    GLuint shader_program = LoadShaders(window, vs_path.c_str(), ps_path.c_str());



    // vao and vbo handle
    GLuint vao, vbo;

    // generate and bind the vao
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // generate and bind the buffer object
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // data for a fullscreen quad
    GLfloat vertexData[] = {
        //  X     Y     Z           R     G     B
        1.0f, 1.0f, 0.0f,       1.0f, 0.0f, 0.0f, // vertex 0
        -1.0f, 1.0f, 0.0f,       0.0f, 1.0f, 0.0f, // vertex 1
        1.0f,-1.0f, 0.0f,       0.0f, 0.0f, 1.0f, // vertex 2
        1.0f,-1.0f, 0.0f,       0.0f, 0.0f, 1.0f, // vertex 3
        -1.0f, 1.0f, 0.0f,       0.0f, 1.0f, 0.0f, // vertex 4
        -1.0f,-1.0f, 0.0f,       1.0f, 0.0f, 0.0f, // vertex 5
    }; // 6 vertices with 6 components (floats) each

    // fill with data
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*6*6, vertexData, GL_STATIC_DRAW);

    // set up generic attrib pointers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (char*)0 + 0*sizeof(GLfloat));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (char*)0 + 3*sizeof(GLfloat));

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // clear first
        glClear(GL_COLOR_BUFFER_BIT);

        // use the shader program
        glUseProgram(shader_program);

        // bind the vao
        glBindVertexArray(vao);

        // draw
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // check for errors
        GLenum error = glGetError();
        if(error != GL_NO_ERROR) {
            std::cerr << error << std::endl;
            break;
        }

        // finally swap buffers
        glfwSwapBuffers(window);
    }

    // delete the created objects

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    glDeleteProgram(shader_program);

    glfwDestroyWindow(window);
    glfwTerminate();
    return true;
}