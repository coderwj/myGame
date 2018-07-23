#include <iostream>
#include <Windows.h>
#include <windowsx.h>
#include <assert.h>

#include "GameClient.h"

using namespace myGame;

static GameClient * g_client = NULL;

static int g_window_width = 960;
static int g_window_height = 540;

extern HWND g_window = NULL;

void onFramebufferResize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
	GameClient * pGameClient = GameClient::getInstance();
	if (nullptr != pGameClient)
	{
		pGameClient->onResize(width, height);
	}
}

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	GameClient * pGameClient = GameClient::getInstance();
	if (nullptr != pGameClient)
	{
		pGameClient->onMouseScroll(static_cast<float>(yoffset));
	}
}

bool initGlfw()
{
    if(glfwInit() == GL_FALSE)
        return false;

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

    glfwSetFramebufferSizeCallback(window, onFramebufferResize);
	glfwSetScrollCallback(window, scroll_callback);

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

	GameClient* pGameClient = GameClient::getInstance();
	assert(NULL != pGameClient);
	pGameClient->init();
    if(!initGlfw())
    {
        return 0;
    }

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

        processInput(window);

        pGameScene->tick(deltaTime);
		pDebugInfo->tick(deltaTime);


		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		pGameScene->render();
		pDebugInfo->render();

        glfwSwapBuffers(window);
        glfwPollEvents();

        // check for errors
        glCheckError();
    }
    return 0;
}