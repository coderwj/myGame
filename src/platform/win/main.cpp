#include <iostream>
#include <Windows.h>
#include <windowsx.h>
#include <assert.h>

#include "GameClient.h"

using namespace myGame;

static GameClient * g_client = NULL;

static int g_window_width = 960;
static int g_window_height = 540;

static bool g_mouse_left_down = false;
static bool g_mouse_right_down = false;

extern HWND g_window = NULL;

void checkScreenSize()
{

	RECT window_rect;
	GetClientRect(g_window, &window_rect);
	
	int width = window_rect.right - window_rect.left;
	int height = window_rect.bottom - window_rect.top;

	if (width == 0 || height == 0)
		return;
	if (width != g_window_width || height != g_window_height)
	{
		if (nullptr != g_client)
			g_client->onResize(width, height);
	}
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		g_window = hWnd;
	}
	break;
	case WM_CHAR:
	{
		g_client->handleCharInput(static_cast<int>(GET_WPARAM(wParam, lParam)));
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_KEYDOWN:
	{
		g_client->handleKeyDown(static_cast<int>(GET_WPARAM(wParam, lParam)));
	}
	break;
	case WM_KEYUP:
	{
		g_client->handleKeyUp(static_cast<int>(GET_WPARAM(wParam, lParam)));
	}
	break;

	case WM_MOUSEWHEEL:
	{
		bool b_mouse_wheel_down = (GET_X_LPARAM(wParam) != 0);
		int scroll_delta = GET_Y_LPARAM(wParam);
		int pos_x = GET_X_LPARAM(lParam);
		int pos_y = GET_Y_LPARAM(lParam);
		g_client->handleMouseWheel(b_mouse_wheel_down, scroll_delta, pos_x, pos_y);
	}
	break;

	case WM_LBUTTONDOWN:
	{
		g_client->handleTouchBegin(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		g_mouse_left_down = true;
	}
	break;
	case WM_LBUTTONUP:
	{
		g_client->handleTouchEnd(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		g_mouse_left_down = false;
	}
	break;
	case WM_RBUTTONDOWN:
	{

	}
	break;
	case WM_RBUTTONUP:
	{

	}
	break;
	case WM_MOUSEMOVE:
	{
		if (g_mouse_left_down)
			g_client->handleTouchMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	}
	break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int __stdcall WinMain(_In_ HINSTANCE h, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	//char name[512];
	//GetModuleFileName(NULL, name, sizeof(name));
	////TODO, set name
	//SetCurrentDirectory(name);

	HINSTANCE hInstance = GetModuleHandle(NULL);
	char appName[] = "myGameWin32";

	WNDCLASS cs = { 0 };
	cs.cbClsExtra = 0;
	cs.cbWndExtra = 0;
	cs.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	cs.hCursor = LoadCursor(NULL, IDC_ARROW);
	cs.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	cs.hInstance = hInstance;
	cs.lpfnWndProc = (WNDPROC)WndProc;
	cs.lpszClassName = appName;
	cs.lpszMenuName = NULL;
	cs.style = CS_VREDRAW | CS_HREDRAW;
	RegisterClass(&cs);

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int windowLeft = (screenWidth - g_window_width) / 2;
	int windowTop = (screenHeight - g_window_height) / 2;

	//create window.
	HWND hWnd = CreateWindow(appName, "myGame", WS_OVERLAPPEDWINDOW, windowLeft, windowTop, g_window_width, g_window_height, NULL, NULL, hInstance, NULL);

	//show window.
	ShowWindow(hWnd, SW_SHOW);

	//resize window.
	RECT client_rect, window_rect;
	GetClientRect(hWnd, &client_rect);
	GetWindowRect(hWnd, &window_rect);

	int diff_x = window_rect.right - window_rect.left - client_rect.right;
	int diff_y = window_rect.bottom - window_rect.top - client_rect.bottom;

	MoveWindow(hWnd, window_rect.left, window_rect.top, g_window_width + diff_x, g_window_height + diff_y, TRUE);

	UpdateWindow(hWnd);

	ShowCursor(true);

	//msg
	MSG nMsg = { 0 };
	while (WM_QUIT != nMsg.message)
	{
		if (PeekMessage(&nMsg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&nMsg);
			DispatchMessage(&nMsg);
		}
		else
		{
			static bool isInited = false;
			if (!isInited)
			{
				g_client = GameClient::getInstance();
				g_client->init();
				isInited = true;
			}
			g_client->tick();

			checkScreenSize();
		}
	}
	return 0;
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

    while(!glfwWindowShouldClose(window))
    {

        processInput(window);

        pGameScene->tick(deltaTime);
    }
    return 0;
}