#import "AppDelegate.h"
#include "bgfx/platform.h"

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <QuartzCore/CAEAGLLayer.h>

static    void* m_device = NULL;

@interface View : UIView
{
    CADisplayLink* m_displayLink;
}

@end

@implementation View

+ (Class)layerClass
{
#ifdef HAS_METAL_SDK
    Class metalClass = NSClassFromString(@"CAMetalLayer");    //is metal runtime sdk available
    if ( metalClass != nil)
    {
        m_device = MTLCreateSystemDefaultDevice(); // is metal supported on this device (is there a better way to do this - without creating device ?)
        if (m_device)
        {
            [m_device retain];
            return metalClass;
        }
    }
#endif
    
    return [CAEAGLLayer class];
}

- (id)initWithFrame:(CGRect)rect
{
    self = [super initWithFrame:rect];
    
    if (nil == self)
    {
        return nil;
    }
    
    bgfx::PlatformData pd;
    pd.ndt          = NULL;
    pd.nwh          = (__bridge void*)(self.layer);
    pd.context      = NULL;
    pd.backBuffer   = NULL;
    pd.backBufferDS = NULL;
    bgfx::setPlatformData(pd);
    
    return self;
}

- (void)layoutSubviews
{
//    uint32_t frameW = (uint32_t)(self.contentScaleFactor * self.frame.size.width);
//    uint32_t frameH = (uint32_t)(self.contentScaleFactor * self.frame.size.height);
//    s_ctx->m_eventQueue.postSizeEvent(s_defaultWindow, frameW, frameH);
}

- (void)start
{
    if (nil == m_displayLink)
    {
        m_displayLink = [self.window.screen displayLinkWithTarget:self selector:@selector(renderFrame)];
        //[m_displayLink setFrameInterval:1];
        //[m_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        //        [m_displayLink addToRunLoop:[NSRunLoop currentRunLoop]];
        [m_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
    }
}

- (void)stop
{
    if (nil != m_displayLink)
    {
        [m_displayLink invalidate];
        m_displayLink = nil;
    }
}

- (void)renderFrame
{
    
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint touchLocation = [touch locationInView:self];
    touchLocation.x *= self.contentScaleFactor;
    touchLocation.y *= self.contentScaleFactor;
    
//    s_ctx->m_eventQueue.postMouseEvent(s_defaultWindow, touchLocation.x, touchLocation.y, 0);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint touchLocation = [touch locationInView:self];
    touchLocation.x *= self.contentScaleFactor;
    touchLocation.y *= self.contentScaleFactor;
    
//    s_ctx->m_eventQueue.postMouseEvent(s_defaultWindow, touchLocation.x, touchLocation.y, 0, MouseButton::Left, false);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint touchLocation = [touch locationInView:self];
    touchLocation.x *= self.contentScaleFactor;
    touchLocation.y *= self.contentScaleFactor;
    
//    s_ctx->m_eventQueue.postMouseEvent(s_defaultWindow, touchLocation.x, touchLocation.y, 0);
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint touchLocation = [touch locationInView:self];
    touchLocation.x *= self.contentScaleFactor;
    touchLocation.y *= self.contentScaleFactor;
    
//    s_ctx->m_eventQueue.postMouseEvent(s_defaultWindow, touchLocation.x, touchLocation.y, 0, MouseButton::Left, false);
}

@end

@interface AppDelegate()

@end

@implementation AppDelegate

@synthesize m_window;
@synthesize m_view;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
  // Insert code here to initialize your application
  NSLog(@"didFinishLaunchingWithOptions");
    
    
    CGRect rect = [ [UIScreen mainScreen] bounds];
    m_window = [ [UIWindow alloc] initWithFrame: rect];
    m_view = [ [View alloc] initWithFrame: rect];
    
    [m_window addSubview: m_view];
    
    UIViewController *viewController = [[UIViewController alloc] init];
    viewController.view = m_view;
    
    [m_window setRootViewController:viewController];
    [m_window makeKeyAndVisible];
    
    float scaleFactor = [[UIScreen mainScreen] scale];
    [m_view setContentScaleFactor: scaleFactor ];
    
  return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}


@end
