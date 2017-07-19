#include <iostream>
#include "MyEngineCore.h"
#include "gamescene.h"

static GLFWwindow * window = NULL;
static int width = 640;
static int height = 480;

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
    if(fov >= 1.0f && fov <= 90.0f)
        fov -= float(yoffset);
    if(fov <= 1.0f)
        fov = 1.0f;
    if(fov >= 90.0f)
        fov = 90.0f;
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

    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
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

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_A)
        {
            speed_r_y = -1.0f;
        }
        if (key == GLFW_KEY_D)
        {
            speed_r_y = 1.0f;
        }
    }
    else if(action == GLFW_RELEASE)
    {
        speed_r_y = 0.0f;
    }
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
    std::cout << "-----------GameScene start-----------" << std::endl;
    return 0;
}