//
//  AppDelegate.m
//  abc
//
//  Created by 文杰 on 2019/3/29.
//  Copyright © 2019年 文杰. All rights reserved.
//

#import "AppDelegate.h"

#include "GameClient.h"
#include "bgfx/platform.h"

int g_width = 1280, g_height = 720;

@interface MyOpenGLView : NSOpenGLView
{
    NSTimer*    animationTimer;
}
@end

@implementation MyOpenGLView

-(void)animationTimerFired:(NSTimer*)timer
{
    [self setNeedsDisplay:YES];
}

-(void)reshape
{
    [[self openGLContext] update];
    CGSize size = [[self superview] bounds].size;
    g_width = size.width;
    g_height = size.height;
    myGame::GameClient* client = myGame::GameClient::getInstance();
    if(nullptr != client)
    {
        client->onResize(g_width, g_height);
    }
}

-(void)drawRect:(NSRect)bounds
{
    myGame::GameClient* client = myGame::GameClient::getInstance();
    if(nullptr != client)
    {
        client->tick();
    }

    if (!animationTimer)
        animationTimer = [NSTimer scheduledTimerWithTimeInterval:0.017 target:self selector:@selector(animationTimerFired:) userInfo:nil repeats:YES];
}

-(BOOL)acceptsFirstResponder
{
    return (YES);
}

-(BOOL)becomeFirstResponder
{
    return (YES);
}

-(BOOL)resignFirstResponder
{
    return (YES);
}

// Flip coordinate system upside down on Y
-(BOOL)isFlipped
{
    return (YES);
}

-(void)dealloc
{
    [super dealloc];
    animationTimer = nil;
}

-(void)keyUp:(NSEvent *)event
{
    //ImGui_ImplOSX_HandleEvent(event, self);
}
-(void)keyDown:(NSEvent *)event
{
    //ImGui_ImplOSX_HandleEvent(event, self);
    myGame::GameClient* client = myGame::GameClient::getInstance();
    if (nullptr == client)
        return;
    
    NSString* str = [event characters];
    int len = (int)[str length];
    for (int i = 0; i < len; i++)
    {
        int c = [str characterAtIndex:i];
        client->handleKeyDown(c);
    }
}
-(void)flagsChanged:(NSEvent *)event
{
}
-(void)mouseDown:(NSEvent *)event
{
    NSPoint pos = [event locationInWindow];
    pos.y = g_height - pos.y;
    myGame::GameClient* client = myGame::GameClient::getInstance();
    if (nullptr != client)
        client->handleMouseDown(static_cast<int>(pos.x), static_cast<int>(pos.y));
}
-(void)mouseUp:(NSEvent *)event
{
    NSPoint pos = [event locationInWindow];
    pos.y = g_height - pos.y;
    myGame::GameClient* client = myGame::GameClient::getInstance();
    if (nullptr != client)
        client->handleMouseUp(static_cast<int>(pos.x), static_cast<int>(pos.y));
}

-(void)mouseDragged:(NSEvent *)event
{
    NSPoint pos = [event locationInWindow];
    pos.y = g_height - pos.y;
    myGame::GameClient* client = myGame::GameClient::getInstance();
    if (nullptr != client)
        client->handleMouseDragged(static_cast<int>(pos.x), static_cast<int>(pos.y));
}

-(void)scrollWheel:(NSEvent *)event
{
    double wheel_dy = [event deltaY];
    myGame::GameClient* client = myGame::GameClient::getInstance();
    if (nullptr != client)
        client->handleMouseWheel(false, wheel_dy, 0, 0);
}

@end


@implementation EditorAppDelegate

@synthesize window = _window;

-(NSWindow*)window
{
    if (_window != nil)
        return (_window);
    
    NSRect viewRect = NSMakeRect(0, 0, g_width, g_height);
    _window = [[NSWindow alloc] initWithContentRect:viewRect styleMask:NSWindowStyleMaskTitled|NSWindowStyleMaskMiniaturizable|NSWindowStyleMaskResizable|NSWindowStyleMaskClosable|NSWindowStyleMaskFullSizeContentView backing:NSBackingStoreBuffered defer:YES];
    [_window setTitle:@"Editor"];
    [_window setOpaque:YES];
    [_window makeKeyAndOrderFront:NSApp];
    
    return (_window);
}

-(void)setupMenu
{
    NSMenu* mainMenuBar = [[NSMenu alloc] init];
    NSMenu* appMenu;
    NSMenuItem* menuItem;
    
    appMenu = [[NSMenu alloc] initWithTitle:@"Editor"];
    menuItem = [appMenu addItemWithTitle:@"Quit Editor" action:@selector(terminate:) keyEquivalent:@"q"];
    [menuItem setKeyEquivalentModifierMask:NSEventModifierFlagCommand];
    
    menuItem = [[NSMenuItem alloc] init];
    [menuItem setSubmenu:appMenu];
    
    [mainMenuBar addItem:menuItem];
    
    appMenu = nil;
    [NSApp setMainMenu:mainMenuBar];
}

-(void)dealloc
{
    [super dealloc];
    _window = nil;
}


inline void osxSetNSWindow(void* _window, void* _nsgl)
{
    bgfx::PlatformData pd;
    pd.ndt          = NULL;
    pd.nwh          = _window;
    pd.context      = _nsgl;
    pd.backBuffer   = NULL;
    pd.backBufferDS = NULL;
    bgfx::setPlatformData(pd);
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
    
    // Make the application a foreground application (else it won't receive keyboard events)
    ProcessSerialNumber psn = {0, kCurrentProcess};
    TransformProcessType(&psn, kProcessTransformToForegroundApplication);
    
    // Menu
    [self setupMenu];
    
    NSOpenGLPixelFormatAttribute attributes[] = {
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
        NSOpenGLPFAColorSize,     32,
        NSOpenGLPFAAlphaSize,     8,
        NSOpenGLPFADepthSize,     24,
        NSOpenGLPFAStencilSize,   8,
        NSOpenGLPFADoubleBuffer,  true,
        NSOpenGLPFAAccelerated,   true,
        NSOpenGLPFANoRecovery,    true,
        0,                        0,
    };
    NSOpenGLPixelFormat *pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes];
    MyOpenGLView* view = [[MyOpenGLView alloc] initWithFrame:self.window.frame pixelFormat:pixelFormat];
    [pixelFormat release];
    [self.window setContentView:view];
    
    if ([view openGLContext] == nil)
        NSLog(@"No OpenGL Context!");
    
    osxSetNSWindow(self.window, [view openGLContext]);
    
    
    myGame::GameClient* client = myGame::GameClient::getInstance();
    client->init();
    client->onResize(g_width, g_height);
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}

@end
