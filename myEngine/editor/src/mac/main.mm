//
//  main.m
//  abc
//
//  Created by 文杰 on 2019/3/29.
//  Copyright © 2019年 文杰. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "AppDelegate.h"


int main(int argc, const char * argv[]) {
    @autoreleasepool
    {
        NSApp = [NSApplication sharedApplication];
        EditorAppDelegate* delegate = [[EditorAppDelegate alloc] init];
        [[NSApplication sharedApplication] setDelegate:delegate];
        [NSApp run];
    }
    return NSApplicationMain(argc, argv);
}
