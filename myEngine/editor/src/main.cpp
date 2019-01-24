#ifdef WIN32
#include <Windows.h>
#include <windowsx.h>
#else
#import <Cocoa/Cocoa.h>
#endif
#include <assert.h>
#include <iostream>

#include "GameClient.h"

#include "bgfx/platform.h"

using namespace myGame;

static GameClient * g_client = NULL;

static int g_window_width = 1280;
static int g_window_height = 720;


#ifdef WIN32

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
	char appName[] = "myEditor";

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
	HWND hWnd = CreateWindow(appName, "myEditor", WS_OVERLAPPEDWINDOW, windowLeft, windowTop, g_window_width, g_window_height, NULL, NULL, hInstance, NULL);

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

#else




@interface AppDelegate : NSObject<NSApplicationDelegate>
{
    bool terminated;
}

+ (AppDelegate *)sharedDelegate;
- (id)init;
- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender;
- (bool)applicationHasTerminated;

@end

@interface Window : NSObject<NSWindowDelegate>
{
    uint32_t windowCount;
}

+ (Window*)sharedDelegate;
- (id)init;
- (void)windowCreated:(NSWindow*)window;
- (void)windowWillClose:(NSNotification*)notification;
- (BOOL)windowShouldClose:(NSWindow*)window;
- (void)windowDidResize:(NSNotification*)notification;
- (void)windowDidBecomeKey:(NSNotification *)notification;
- (void)windowDidResignKey:(NSNotification *)notification;

@end

inline void osxSetNSWindow(void* _window, void* _nsgl = NULL)
{
    bgfx::PlatformData pd;
    pd.ndt          = NULL;
    pd.nwh          = _window;
    pd.context      = _nsgl;
    pd.backBuffer   = NULL;
    pd.backBufferDS = NULL;
    bgfx::setPlatformData(pd);
}

static WindowHandle s_defaultWindow = { 0 };
static uint8_t s_translateKey[256];

struct MainThreadEntry
{
    int m_argc;
    const char* const* m_argv;
    
    static int32_t threadFunc(bx::Thread* _thread, void* _userData)
    {
        BX_UNUSED(_thread);
        
        CFBundleRef mainBundle = CFBundleGetMainBundle();
        if (mainBundle != nil)
        {
            CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
            if (resourcesURL != nil)
            {
                char path[PATH_MAX];
                if (CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8*)path, PATH_MAX) )
                {
                    chdir(path);
                }
                
                CFRelease(resourcesURL);
            }
        }
        
        MainThreadEntry* self = (MainThreadEntry*)_userData;
        return main(self->m_argc, self->m_argv);
    }
};

struct Context
{
    Context()
    : m_scrollf(0.0f)
    , m_mx(0)
    , m_my(0)
    , m_scroll(0)
    , m_style(0)
    , m_exit(false)
    , m_fullscreen(false)
    {
        s_translateKey[27]             = Key::Esc;
        s_translateKey[uint8_t('\r')]  = Key::Return;
        s_translateKey[uint8_t('\t')]  = Key::Tab;
        s_translateKey[127]            = Key::Backspace;
        s_translateKey[uint8_t(' ')]   = Key::Space;
        
        s_translateKey[uint8_t('+')]   =
        s_translateKey[uint8_t('=')]   = Key::Plus;
        s_translateKey[uint8_t('_')]   =
        s_translateKey[uint8_t('-')]   = Key::Minus;
        
        s_translateKey[uint8_t('~')]   =
        s_translateKey[uint8_t('`')]   = Key::Tilde;
        
        s_translateKey[uint8_t(':')]   =
        s_translateKey[uint8_t(';')]   = Key::Semicolon;
        s_translateKey[uint8_t('"')]   =
        s_translateKey[uint8_t('\'')]  = Key::Quote;
        
        s_translateKey[uint8_t('{')]   =
        s_translateKey[uint8_t('[')]   = Key::LeftBracket;
        s_translateKey[uint8_t('}')]   =
        s_translateKey[uint8_t(']')]   = Key::RightBracket;
        
        s_translateKey[uint8_t('<')]   =
        s_translateKey[uint8_t(',')]   = Key::Comma;
        s_translateKey[uint8_t('>')]   =
        s_translateKey[uint8_t('.')]   = Key::Period;
        s_translateKey[uint8_t('?')]   =
        s_translateKey[uint8_t('/')]   = Key::Slash;
        s_translateKey[uint8_t('|')]   =
        s_translateKey[uint8_t('\\')]  = Key::Backslash;
        
        s_translateKey[uint8_t('0')]   = Key::Key0;
        s_translateKey[uint8_t('1')]   = Key::Key1;
        s_translateKey[uint8_t('2')]   = Key::Key2;
        s_translateKey[uint8_t('3')]   = Key::Key3;
        s_translateKey[uint8_t('4')]   = Key::Key4;
        s_translateKey[uint8_t('5')]   = Key::Key5;
        s_translateKey[uint8_t('6')]   = Key::Key6;
        s_translateKey[uint8_t('7')]   = Key::Key7;
        s_translateKey[uint8_t('8')]   = Key::Key8;
        s_translateKey[uint8_t('9')]   = Key::Key9;
        
        for (char ch = 'a'; ch <= 'z'; ++ch)
        {
            s_translateKey[uint8_t(ch)]       =
            s_translateKey[uint8_t(ch - ' ')] = Key::KeyA + (ch - 'a');
        }
    }
    
    NSEvent* waitEvent()
    {
        return [NSApp
                nextEventMatchingMask:NSEventMaskAny
                untilDate:[NSDate distantFuture] // wait for event
                inMode:NSDefaultRunLoopMode
                dequeue:YES
                ];
    }
    
    NSEvent* peekEvent()
    {
        return [NSApp
                nextEventMatchingMask:NSEventMaskAny
                untilDate:[NSDate distantPast] // do not wait for event
                inMode:NSDefaultRunLoopMode
                dequeue:YES
                ];
    }
    
    void getMousePos(int32_t* outX, int32_t* outY)
    {
        WindowHandle handle = { 0 };
        NSWindow* window = m_window[handle.idx];
        
        NSRect  originalFrame = [window frame];
        NSPoint location      = [window mouseLocationOutsideOfEventStream];
        NSRect  adjustFrame   = [window contentRectForFrameRect: originalFrame];
        
        int32_t x = location.x;
        int32_t y = int32_t(adjustFrame.size.height) - int32_t(location.y);
        
        // clamp within the range of the window
        *outX = bx::clamp(x, 0, int32_t(adjustFrame.size.width) );
        *outY = bx::clamp(y, 0, int32_t(adjustFrame.size.height) );
    }
    
    uint8_t translateModifiers(int flags)
    {
        return 0
        | (0 != (flags & NSEventModifierFlagShift  ) ) ? Modifier::LeftShift | Modifier::RightShift : 0
        | (0 != (flags & NSEventModifierFlagOption ) ) ? Modifier::LeftAlt   | Modifier::RightAlt   : 0
        | (0 != (flags & NSEventModifierFlagControl) ) ? Modifier::LeftCtrl  | Modifier::RightCtrl  : 0
        | (0 != (flags & NSEventModifierFlagCommand) ) ? Modifier::LeftMeta  | Modifier::RightMeta  : 0
        ;
    }
    
    Key::Enum handleKeyEvent(NSEvent* event, uint8_t* specialKeys, uint8_t* _pressedChar)
    {
        NSString* key = [event charactersIgnoringModifiers];
        unichar keyChar = 0;
        if ([key length] == 0)
        {
            return Key::None;
        }
        
        keyChar = [key characterAtIndex:0];
        *_pressedChar = (uint8_t)keyChar;
        
        int keyCode = keyChar;
        *specialKeys = translateModifiers(int([event modifierFlags]));
        
        // if this is a unhandled key just return None
        if (keyCode < 256)
        {
            return (Key::Enum)s_translateKey[keyCode];
        }
        
        switch (keyCode)
        {
                case NSF1FunctionKey:  return Key::F1;
                case NSF2FunctionKey:  return Key::F2;
                case NSF3FunctionKey:  return Key::F3;
                case NSF4FunctionKey:  return Key::F4;
                case NSF5FunctionKey:  return Key::F5;
                case NSF6FunctionKey:  return Key::F6;
                case NSF7FunctionKey:  return Key::F7;
                case NSF8FunctionKey:  return Key::F8;
                case NSF9FunctionKey:  return Key::F9;
                case NSF10FunctionKey: return Key::F10;
                case NSF11FunctionKey: return Key::F11;
                case NSF12FunctionKey: return Key::F12;
                
                case NSLeftArrowFunctionKey:   return Key::Left;
                case NSRightArrowFunctionKey:  return Key::Right;
                case NSUpArrowFunctionKey:     return Key::Up;
                case NSDownArrowFunctionKey:   return Key::Down;
                
                case NSPageUpFunctionKey:      return Key::PageUp;
                case NSPageDownFunctionKey:    return Key::PageDown;
                case NSHomeFunctionKey:        return Key::Home;
                case NSEndFunctionKey:         return Key::End;
                
                case NSPrintScreenFunctionKey: return Key::Print;
        }
        
        return Key::None;
    }
    
    bool dispatchEvent(NSEvent* event)
    {
        if (event)
        {
            NSEventType eventType = [event type];
            
            switch (eventType)
            {
                    case NSEventTypeMouseMoved:
                    case NSEventTypeLeftMouseDragged:
                    case NSEventTypeRightMouseDragged:
                    case NSEventTypeOtherMouseDragged:
                    getMousePos(&m_mx, &m_my);
                    m_eventQueue.postMouseEvent(s_defaultWindow, m_mx, m_my, m_scroll);
                    break;
                    
                    case NSEventTypeLeftMouseDown:
                {
                    // Command + Left Mouse Button acts as middle! This just a temporary solution!
                    // This is because the average OSX user doesn't have middle mouse click.
                    MouseButton::Enum mb = ([event modifierFlags] & NSEventModifierFlagCommand)
                    ? MouseButton::Middle
                    : MouseButton::Left
                    ;
                    m_eventQueue.postMouseEvent(s_defaultWindow, m_mx, m_my, m_scroll, mb, true);
                }
                    break;
                    
                    case NSEventTypeLeftMouseUp:
                    m_eventQueue.postMouseEvent(s_defaultWindow, m_mx, m_my, m_scroll, MouseButton::Left, false);
                    m_eventQueue.postMouseEvent(s_defaultWindow, m_mx, m_my, m_scroll, MouseButton::Middle, false);
                    break;
                    
                    case NSEventTypeRightMouseDown:
                    m_eventQueue.postMouseEvent(s_defaultWindow, m_mx, m_my, m_scroll, MouseButton::Right, true);
                    break;
                    
                    case NSEventTypeRightMouseUp:
                    m_eventQueue.postMouseEvent(s_defaultWindow, m_mx, m_my, m_scroll, MouseButton::Right, false);
                    break;
                    
                    case NSEventTypeOtherMouseDown:
                    m_eventQueue.postMouseEvent(s_defaultWindow, m_mx, m_my, m_scroll, MouseButton::Middle, true);
                    break;
                    
                    case NSEventTypeOtherMouseUp:
                    m_eventQueue.postMouseEvent(s_defaultWindow, m_mx, m_my, m_scroll, MouseButton::Middle, false);
                    break;
                    
                    case NSEventTypeScrollWheel:
                    m_scrollf += [event deltaY];
                    
                    m_scroll = (int32_t)m_scrollf;
                    m_eventQueue.postMouseEvent(s_defaultWindow, m_mx, m_my, m_scroll);
                    break;
                    
                    case NSEventTypeKeyDown:
                {
                    uint8_t modifiers = 0;
                    uint8_t pressedChar[4];
                    Key::Enum key = handleKeyEvent(event, &modifiers, &pressedChar[0]);
                    
                    // Returning false means that we take care of the key (instead of the default behavior)
                    if (key != Key::None)
                    {
                        if (key == Key::KeyQ && (modifiers & Modifier::RightMeta) )
                        {
                            m_eventQueue.postExitEvent();
                        }
                        else
                        {
                            enum { ShiftMask = Modifier::LeftShift|Modifier::RightShift };
                            m_eventQueue.postCharEvent(s_defaultWindow, 1, pressedChar);
                            m_eventQueue.postKeyEvent(s_defaultWindow, key, modifiers, true);
                            return false;
                        }
                    }
                }
                    break;
                    
                    case NSEventTypeKeyUp:
                {
                    uint8_t modifiers  = 0;
                    uint8_t pressedChar[4];
                    Key::Enum key = handleKeyEvent(event, &modifiers, &pressedChar[0]);
                    
                    BX_UNUSED(pressedChar);
                    
                    if (key != Key::None)
                    {
                        m_eventQueue.postKeyEvent(s_defaultWindow, key, modifiers, false);
                        return false;
                    }
                    
                }
                    break;
                    
                default:
                    break;
            }
            
            [NSApp sendEvent:event];
            [NSApp updateWindows];
            
            return true;
        }
        
        return false;
    }
    
    void windowDidResize()
    {
        WindowHandle handle = { 0 };
        NSWindow* window = m_window[handle.idx];
        NSRect originalFrame = [window frame];
        NSRect rect = [window contentRectForFrameRect: originalFrame];
        uint32_t width  = uint32_t(rect.size.width);
        uint32_t height = uint32_t(rect.size.height);
        m_eventQueue.postSizeEvent(handle, width, height);
        
        // Make sure mouse button state is 'up' after resize.
        m_eventQueue.postMouseEvent(s_defaultWindow, m_mx, m_my, m_scroll, MouseButton::Left,  false);
        m_eventQueue.postMouseEvent(s_defaultWindow, m_mx, m_my, m_scroll, MouseButton::Right, false);
    }
    
    void windowDidBecomeKey()
    {
        m_eventQueue.postSuspendEvent(s_defaultWindow, Suspend::WillResume);
        m_eventQueue.postSuspendEvent(s_defaultWindow, Suspend::DidResume);
    }
    
    void windowDidResignKey()
    {
        m_eventQueue.postSuspendEvent(s_defaultWindow, Suspend::WillSuspend);
        m_eventQueue.postSuspendEvent(s_defaultWindow, Suspend::DidSuspend);
    }
    
    int32_t run(int _argc, const char* const* _argv)
    {
        [NSApplication sharedApplication];
        
        id dg = [AppDelegate sharedDelegate];
        [NSApp setDelegate:dg];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        [NSApp activateIgnoringOtherApps:YES];
        [NSApp finishLaunching];
        
        [[NSNotificationCenter defaultCenter]
         postNotificationName:NSApplicationWillFinishLaunchingNotification
         object:NSApp];
        
        [[NSNotificationCenter defaultCenter]
         postNotificationName:NSApplicationDidFinishLaunchingNotification
         object:NSApp];
        
        id quitMenuItem = [NSMenuItem new];
        [quitMenuItem
         initWithTitle:@"Quit"
         action:@selector(terminate:)
         keyEquivalent:@"q"];
        
        id appMenu = [NSMenu new];
        [appMenu addItem:quitMenuItem];
        
        id appMenuItem = [NSMenuItem new];
        [appMenuItem setSubmenu:appMenu];
        
        id menubar = [[NSMenu new] autorelease];
        [menubar addItem:appMenuItem];
        [NSApp setMainMenu:menubar];
        
        m_style = 0
        | NSWindowStyleMaskTitled
        | NSWindowStyleMaskResizable
        | NSWindowStyleMaskClosable
        | NSWindowStyleMaskMiniaturizable
        ;
        
        NSRect screenRect = [[NSScreen mainScreen] frame];
        const float centerX = (screenRect.size.width  - (float)ENTRY_DEFAULT_WIDTH )*0.5f;
        const float centerY = (screenRect.size.height - (float)ENTRY_DEFAULT_HEIGHT)*0.5f;
        
        m_windowAlloc.alloc();
        NSRect rect = NSMakeRect(centerX, centerY, ENTRY_DEFAULT_WIDTH, ENTRY_DEFAULT_HEIGHT);
        NSWindow* window = [[NSWindow alloc]
                            initWithContentRect:rect
                            styleMask:m_style
                            backing:NSBackingStoreBuffered defer:NO
                            ];
        NSString* appName = [[NSProcessInfo processInfo] processName];
        [window setTitle:appName];
        [window makeKeyAndOrderFront:window];
        [window setAcceptsMouseMovedEvents:YES];
        [window setBackgroundColor:[NSColor blackColor]];
        [[Window sharedDelegate] windowCreated:window];
        
        m_window[0] = window;
        m_windowFrame = [window frame];
        
        osxSetNSWindow(window);
        
        MainThreadEntry mte;
        mte.m_argc = _argc;
        mte.m_argv = _argv;
        
        bx::Thread thread;
        thread.init(mte.threadFunc, &mte);
        
        WindowHandle handle = { 0 };
        NSRect contentRect = [window contentRectForFrameRect: m_windowFrame];
        uint32_t width = uint32_t(contentRect.size.width);
        uint32_t height = uint32_t(contentRect.size.height);
        m_eventQueue.postSizeEvent(handle, width, height);
        
        while (!(m_exit = [dg applicationHasTerminated]) )
        {
            @autoreleasepool
            {
                bgfx::renderFrame();
            }
            
            while (dispatchEvent(peekEvent() ) )
            {
            }
        }
        
        m_eventQueue.postExitEvent();
        
        while (bgfx::RenderFrame::NoContext != bgfx::renderFrame() ) {};
        thread.shutdown();
        
        return 0;
    }
    
    bool isValid(WindowHandle _handle)
    {
        return m_windowAlloc.isValid(_handle.idx);
    }
    
    EventQueue m_eventQueue;
    
    bx::HandleAllocT<ENTRY_CONFIG_MAX_WINDOWS> m_windowAlloc;
    NSWindow* m_window[ENTRY_CONFIG_MAX_WINDOWS];
    NSRect m_windowFrame;
    
    float   m_scrollf;
    int32_t m_mx;
    int32_t m_my;
    int32_t m_scroll;
    int32_t m_style;
    bool    m_exit;
    bool    m_fullscreen;
};

static Context s_ctx;

const Event* poll()
{
    return s_ctx.m_eventQueue.poll();
}

const Event* poll(WindowHandle _handle)
{
    return s_ctx.m_eventQueue.poll(_handle);
}

void release(const Event* _event)
{
    s_ctx.m_eventQueue.release(_event);
}

WindowHandle createWindow(int32_t _x, int32_t _y, uint32_t _width, uint32_t _height, uint32_t _flags, const char* _title)
{
    BX_UNUSED(_x, _y, _width, _height, _flags, _title);
    WindowHandle handle = { UINT16_MAX };
    return handle;
}

void destroyWindow(WindowHandle _handle)
{
    if (s_ctx.isValid(_handle) )
    {
        dispatch_async(dispatch_get_main_queue()
                       , ^{
                           [s_ctx.m_window[_handle.idx] performClose: nil];
                       });
    }
}

void setWindowPos(WindowHandle _handle, int32_t _x, int32_t _y)
{
    if (s_ctx.isValid(_handle) )
    {
        NSWindow* window = s_ctx.m_window[_handle.idx];
        NSScreen* screen = [window screen];
        
        NSRect screenRect = [screen frame];
        CGFloat menuBarHeight = [[[NSApplication sharedApplication] mainMenu] menuBarHeight];
        
        NSPoint position = { float(_x), screenRect.size.height - menuBarHeight - float(_y) };
        
        dispatch_async(dispatch_get_main_queue()
                       , ^{
                           [window setFrameTopLeftPoint: position];
                       });
    }
}

void setWindowSize(WindowHandle _handle, uint32_t _width, uint32_t _height)
{
    if (s_ctx.isValid(_handle) )
    {
        NSSize size = { float(_width), float(_height) };
        dispatch_async(dispatch_get_main_queue()
                       , ^{
                           [s_ctx.m_window[_handle.idx] setContentSize: size];
                       });
    }
}

void setWindowTitle(WindowHandle _handle, const char* _title)
{
    if (s_ctx.isValid(_handle) )
    {
        NSString* title = [[NSString alloc] initWithCString:_title encoding:1];
        dispatch_async(dispatch_get_main_queue()
                       , ^{
                           [s_ctx.m_window[_handle.idx] setTitle: title];
                       });
        [title release];
    }
}

void setWindowFlags(WindowHandle _handle, uint32_t _flags, bool _enabled)
{
    BX_UNUSED(_handle, _flags, _enabled);
}

void toggleFullscreen(WindowHandle _handle)
{
    if (s_ctx.isValid(_handle) )
    {
        NSWindow* window = s_ctx.m_window[_handle.idx];
        NSScreen* screen = [window screen];
        NSRect screenRect = [screen frame];
        
        if (!s_ctx.m_fullscreen)
        {
            s_ctx.m_style &= ~NSWindowStyleMaskTitled;
            dispatch_async(dispatch_get_main_queue()
                           , ^{
                               [NSMenu setMenuBarVisible: false];
                               [window setStyleMask: s_ctx.m_style];
                               [window setFrame:screenRect display:YES];
                           });
            
            s_ctx.m_fullscreen = true;
        }
        else
        {
            s_ctx.m_style |= NSWindowStyleMaskTitled;
            dispatch_async(dispatch_get_main_queue()
                           , ^{
                               [NSMenu setMenuBarVisible: true];
                               [window setStyleMask: s_ctx.m_style];
                               [window setFrame:s_ctx.m_windowFrame display:YES];
                           });
            
            s_ctx.m_fullscreen = false;
        }
    }
}

void setMouseLock(WindowHandle _handle, bool _lock)
{
    BX_UNUSED(_handle, _lock);
}

@implementation AppDelegate

+ (AppDelegate *)sharedDelegate
{
    static id delegate = [AppDelegate new];
    return delegate;
}

- (id)init
{
    self = [super init];
    
    if (nil == self)
    {
        return nil;
    }
    
    self->terminated = false;
    return self;
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender
{
    BX_UNUSED(sender);
    self->terminated = true;
    return NSTerminateCancel;
}

- (bool)applicationHasTerminated
{
    return self->terminated;
}

@end

@implementation Window

+ (Window*)sharedDelegate
{
    static id windowDelegate = [Window new];
    return windowDelegate;
}

- (id)init
{
    self = [super init];
    if (nil == self)
    {
        return nil;
    }
    
    self->windowCount = 0;
    return self;
}

- (void)windowCreated:(NSWindow*)window
{
    assert(window);
    
    [window setDelegate:self];
    
    assert(self->windowCount < ~0u);
    self->windowCount += 1;
}

- (void)windowWillClose:(NSNotification*)notification
{
    BX_UNUSED(notification);
}

- (BOOL)windowShouldClose:(NSWindow*)window
{
    assert(window);
    
    [window setDelegate:nil];
    
    assert(self->windowCount);
    self->windowCount -= 1;
    
    if (self->windowCount == 0)
    {
        [NSApp terminate:self];
        return false;
    }
    
    return true;
}

- (void)windowDidResize:(NSNotification*)notification
{
    BX_UNUSED(notification);
    s_ctx.windowDidResize();
}

- (void)windowDidBecomeKey:(NSNotification*)notification
{
    BX_UNUSED(notification);
    s_ctx.windowDidBecomeKey();
}

- (void)windowDidResignKey:(NSNotification*)notification
{
    BX_UNUSED(notification);
    s_ctx.windowDidResignKey();
}

@end

int main(int _argc, const char* const* _argv)
{
    return s_ctx.run(_argc, _argv);
}




#endif
