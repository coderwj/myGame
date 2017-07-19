#include <iostream>
#include "MyEngineCore.h"
#include "gamescene.h"
#include "Camera.h"

static GLFWwindow * window = NULL;
static int width = 640;
static int height = 480;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
    GameScene * gamescene = GameScene::getInstance();
    if(!gamescene)
        return;
    Camera * camera = gamescene->getCamera();
    if(!camera)
        return;
    camera->ProcessMouseScroll(yoffset);
}

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_A)
        {
        }
        if (key == GLFW_KEY_D)
        {
        }
    }
    else if(action == GLFW_RELEASE)
    {
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

bool initGlfw()
{
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
    if((window = glfwCreateWindow(width, height, "myGame", 0, 0)) == 0) {
        std::cerr << "failed to open window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return true;
}

bool initGlew()
{
    if(glewInit() != GLEW_OK) {
        std::cerr << "failed to init GLEW" << std::endl;
        return false;
    }
    return true;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    GameScene * gamescene = GameScene::getInstance();
    if(!gamescene)
        return;
    Camera * camera = gamescene->getCamera();
    if(!camera)
        return;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(RIGHT, deltaTime);
}

int main(){
    if(!initGlfw())
    {
        std::cout << "initGlfw error!" << std::endl;
        return 0;
    }
    if(!initGlew())
    {
        std::cout << "initGlew error!" << std::endl;
        return 0;
    }

    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    GameScene * pGameScene = GameScene::getInstance();
    if(pGameScene == NULL)
    {
        std::cout << "GameScene Create error!" << std::endl;
        return 0;
    }
    if(!pGameScene->init())
    {
        std::cout << "GameScene Init error!" << std::endl;
        return 0;
    }

    while(!glfwWindowShouldClose(window))
    {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        if(!pGameScene)
        {
            break;
        }
        pGameScene->tick(deltaTime);

        glfwSwapBuffers(window);
        glfwPollEvents();

        // check for errors
        GLenum error = glGetError();
        if(error != GL_NO_ERROR) {
            std::cerr << error << std::endl;
            break;
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}