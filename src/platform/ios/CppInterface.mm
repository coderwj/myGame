#import "CppInterface.h"

#include "bgfx/platform.h"
#include "bgfx/bgfx.h"
#include "GameClient.h"

@interface CppInterface () 
{
    
}
@end

@implementation CppInterface

-(instancetype)init
{
    self = [super init];
    return self;
}

-(void) initGame:(CALayer*)layer
{
    bgfx::PlatformData pd;
    pd.ndt          = NULL;
    pd.nwh          = (__bridge void *)layer;
    pd.context      = NULL;
    pd.backBuffer   = NULL;
    pd.backBufferDS = NULL;
    bgfx::setPlatformData(pd);
    
    NSString* bundlePath =[[NSBundle mainBundle] resourcePath];
    std::string path = [bundlePath UTF8String];
    path += "/";
    myGame::GameClient::getInstance()->setResPath(path.c_str());
    
    myGame::GameClient::getInstance()->init();
}

-(void) tick
{
    myGame::GameClient::getInstance()->tick();
}

@end
