// Objective-C++ objective-c class to interface with C++ Library
@interface CppInterface : NSObject

-(void) initGame:(CALayer*)layer;

-(void) tick;

-(void) handleTouchBegin:(CGPoint)p;
-(void) handleTouchMove:(CGPoint)p;
-(void) handleTouchEnd:(CGPoint)p;

@end
