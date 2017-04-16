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


#include "gamescene.h"

GameScene * GameScene::gs = 0;

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
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
    if((window = glfwCreateWindow(width, height, "00skeleton", 0, 0)) == 0) {
        std::cerr << "failed to open window" << std::endl;
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(window);
    
    // creation and initialization of stuff goes here
    // ...
    
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        // drawing etc goes here
        // ...
        
        // check for errors
        GLenum error = glGetError();
        if(error != GL_NO_ERROR) {
            std::cerr << error << std::endl;
            break;
        }
        
        // finally swap buffers
        glfwSwapBuffers(window);
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return true;

}