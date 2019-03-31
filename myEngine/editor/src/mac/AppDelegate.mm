//
//  AppDelegate.m
//  abc
//
//  Created by 文杰 on 2019/3/29.
//  Copyright © 2019年 文杰. All rights reserved.
//

#import "AppDelegate.h"

#include <assert.h>

#include "GameClient.h"
#include "bgfx/platform.h"


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

-(void)prepareOpenGL
{
    [super prepareOpenGL];
    
#ifndef DEBUG
    GLint swapInterval = 1;
    [[self openGLContext] setValues:&swapInterval forParameter:NSOpenGLCPSwapInterval];
    if (swapInterval == 0)
        NSLog(@"Error: Cannot set swap interval.");
#endif
}

-(void)updateAndDrawDemoView
{
    myGame::GameClient* client = myGame::GameClient::getInstance();
    client->tick();
    
    // Present
    [[self openGLContext] flushBuffer];
    
    if (!animationTimer)
        animationTimer = [NSTimer scheduledTimerWithTimeInterval:0.017 target:self selector:@selector(animationTimerFired:) userInfo:nil repeats:YES];
}

-(void)reshape
{
    [[self openGLContext] update];
    [self updateAndDrawDemoView];
}

-(void)drawRect:(NSRect)bounds
{
    [self updateAndDrawDemoView];
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

// Forward Mouse/Keyboard events to dear imgui OSX back-end. It returns true when imgui is expecting to use the event.
-(void)keyUp:(NSEvent *)event
{
    //ImGui_ImplOSX_HandleEvent(event, self);
}
-(void)keyDown:(NSEvent *)event
{
    //ImGui_ImplOSX_HandleEvent(event, self);
}
-(void)flagsChanged:(NSEvent *)event
{
    //ImGui_ImplOSX_HandleEvent(event, self);
}
-(void)mouseDown:(NSEvent *)event
{
    //ImGui_ImplOSX_HandleEvent(event, self);
}
-(void)mouseUp:(NSEvent *)event
{
    //ImGui_ImplOSX_HandleEvent(event, self);
}
-(void)scrollWheel:(NSEvent *)event
{
    //ImGui_ImplOSX_HandleEvent(event, self);
}

@end




@interface AppDelegate ()

@property (nonatomic, readonly) NSWindow* window;

@end

@implementation AppDelegate

@synthesize window = _window;

-(NSWindow*)window
{
    if (_window != nil)
        return (_window);
    
    NSRect viewRect = NSMakeRect(100.0, 100.0, 100.0 + 1280.0, 100 + 720.0);
    _window = [[NSWindow alloc] initWithContentRect:viewRect styleMask:NSWindowStyleMaskTitled|NSWindowStyleMaskMiniaturizable|NSWindowStyleMaskResizable|NSWindowStyleMaskClosable backing:NSBackingStoreBuffered defer:YES];
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
    
    
    NSOpenGLPixelFormatAttribute attrs[] =
    {
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFADepthSize, 32,
        0
    };
    
    NSOpenGLPixelFormat* format = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
    MyOpenGLView* view = [[MyOpenGLView alloc] initWithFrame:self.window.frame pixelFormat:format];
    format = nil;
#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
    if (floor(NSAppKitVersionNumber) > NSAppKitVersionNumber10_6)
        [view setWantsBestResolutionOpenGLSurface:YES];
#endif // MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
    [self.window setContentView:view];
    
    if ([view openGLContext] == nil)
        NSLog(@"No OpenGL Context!");
    
    osxSetNSWindow(self.window, [view openGLContext]);
    
    
    myGame::GameClient* client = myGame::GameClient::getInstance();
    client->init();
    client->onResize(1280, 720);
    
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}

@end
