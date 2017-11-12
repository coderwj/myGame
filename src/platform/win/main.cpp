#include "glew.h"
#include "glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include <iostream>

#include "Gamescene.h"
#include "Camera.h"

#include "gltools.h"

#ifdef WIN32
    #include <Windows.h>
    #define sleepFunction(t) Sleep(t)
#else
    #define sleepFunction(t)
#endif

static GLFWwindow * window = NULL;
static int width = 640;
static int height = 480;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

static int FPS = 30;

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
    camera->ProcessMouseScroll(GLfloat(yoffset));
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
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera->ProcessKeyboard(ROTATELEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera->ProcessKeyboard(ROTATERIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		camera->ProcessKeyboard(ROTATEUP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		camera->ProcessKeyboard(ROTATEDOWN, deltaTime);
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

    ImGui_ImplGlfwGL3_Init(window, false);

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

        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        if(!pGameScene)
        {
            break;
        }
        pGameScene->tick(deltaTime);

        ImGui_ImplGlfwGL3_NewFrame();
        //ImGui::Text("Hello, world!");
        //ImGui::Render();
        glfwSwapBuffers(window);
        glfwPollEvents();

        // check for errors
        glCheckError();

        double runTime = glfwGetTime() - currentFrame;

        if(runTime * 1000 < 1.0f / FPS * 1000)
            sleepFunction(1.0f / FPS * 1000 - runTime * 1000);
    }
    ImGui_ImplGlfwGL3_Shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}