#import <UIKit/UIApplication.h> // UIApplicationDelegate
#import "ViewController.h"
#import "CppInterface.h"

@interface View : UIView
{
    CADisplayLink* m_displayLink;
}

@end

@interface AppDelegate : UIResponder <UIApplicationDelegate>

+(AppDelegate*) GetDelegate;

@property (strong, nonatomic) UIWindow *m_window;
@property (strong, nonatomic) View *m_view;
@property (strong, nonatomic) ViewController *m_viewController;
@property (strong, nonatomic) CppInterface* m_cppInterface;

@end
