#include <iostream>
#include <Windows.h>
#include <windowsx.h>
#include <assert.h>

#include "GameClient.h"

#include "bgfx/platform.h"

using namespace myGame;

static GameClient * g_client = NULL;

static int g_window_width = 1280;
static int g_window_height = 720;

static bool g_mouse_left_down = false;
static bool g_mouse_right_down = false;

extern HWND g_window = NULL;

void winSetHwnd(HWND _window)
{
	bgfx::PlatformData pd;
	::memset(&pd, 0, sizeof(pd));
	pd.nwh = _window;
	bgfx::setPlatformData(pd);
}

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
		{
			g_client->onResize(width, height);
			g_window_width = width;
			g_window_height = height;
		}
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

	winSetHwnd(hWnd);

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
				g_client->onResize(g_window_width, g_window_height);
				isInited = true;
			}
			g_client->tick();

			checkScreenSize();
		}
	}
	return 0;
}