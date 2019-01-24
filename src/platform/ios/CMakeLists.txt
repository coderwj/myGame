cmake_minimum_required (VERSION 3.7)

message("--->Cmake ios--->" ${CMAKE_MODULE_PATH})


set(CMAKE_CXX_STANDARD 11) #set use C++11
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#set(CMAKE_MODULE_PATH ./cmake)
#set(IOS_PLATFORM SIMULATOR64)
#set(IOS_PLATFORM OS)

OPTION(IOS "" TRUE)

include(cmake/ios.cmake)

set(DEVELOPMENT_PROJECT_NAME    "myGame_project")         # <== Set to your project name, e.g. project.xcodeproj
set(APP_NAME                    "myGame")                 # <== Set To your app's name
set(DEVELOPMENT_TEAM_ID         "None")                   # <== Set to your team ID from Apple
set(APP_BUNDLE_IDENTIFIER       "com.coderwj")            # <== Set to your app's bundle identifier
set(FRAMEWORK_BUNDLE_IDENTIFIER "com.coderwj.framework")  # <== Set to your framework's bundle identifier (cannot be the same as app bundle identifier)
set(CODE_SIGN_IDENTITY          "iPhone Developer")       # <== Set to your preferred code sign identity, to see list:
set(DEPLOYMENT_TARGET           9.0)                      # <== Set your deployment target version of iOS
set(DEVICE_FAMILY               "1,2")                    # <== Set to "1" to target iPhone, set to "2" to target iPad, set to "1,2" to target both
set(LOGIC_ONLY_TESTS            0)                        # <== Set to 1 if you do not want tests to be hosted by the application, speeds up pure 


project(${DEVELOPMENT_PROJECT_NAME})

add_subdirectory(../../../dependencies iosDepend.out)
add_subdirectory(../../gameSrc iosGameSrc.out)
add_subdirectory(../../../myEngine iosmyEngine.out)

set(PRODUCT_NAME                    ${APP_NAME})
set(EXECUTABLE_NAME                 ${APP_NAME})
set(MACOSX_BUNDLE_EXECUTABLE_NAME   ${APP_NAME})
set(MACOSX_BUNDLE_INFO_STRING       ${APP_BUNDLE_IDENTIFIER})
set(MACOSX_BUNDLE_GUI_IDENTIFIER    ${APP_BUNDLE_IDENTIFIER})
set(MACOSX_BUNDLE_BUNDLE_NAME       ${APP_BUNDLE_IDENTIFIER})
set(MACOSX_BUNDLE_ICON_FILE         "")
set(MACOSX_BUNDLE_LONG_VERSION_STRING   "1.0")
set(MACOSX_BUNDLE_SHORT_VERSION_STRING  "1.0")
set(MACOSX_BUNDLE_BUNDLE_VERSION        "1.0")
set(MACOSX_BUNDLE_COPYRIGHT             "Copyright YOU")
set(MACOSX_DEPLOYMENT_TARGET        ${DEPLOYMENT_TARGET})

set(APP_HEADER_FILES
  ./AppDelegate.h
  ./ViewController.h
  ./CppInterface.h
  ./Prefix.pch
)

set(APP_SOURCE_FILES
  ./AppDelegate.m
  ./ViewController.m
  ./CppInterface.mm
  ./main.m
)

set(RESOURCES
  ./Main.storyboard
  ./LaunchScreen.storyboard
  ../../../res/commonConfig.xml
  ../../../res/luafiles
  ../../../res/models
  ../../../res/skybox
  ../../../res/textures
  ../../../myEngine/res/shader
  ../../../myEngine/res/materialConfig.xml
)

add_executable(
    ${APP_NAME}
    MACOSX_BUNDLE
    ${APP_HEADER_FILES}
    ${APP_SOURCE_FILES}
    ${RESOURCES}
)

# To disable bitcode:
# set_target_properties(${APP_NAME} PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE "NO")

include_directories(../../../src/gameSrc)
include_directories(../../../myEngine/src)
#include_directories(../../../../myEngine/lib/bgfx.cmake/bgfx/include)
#include_directories(../../../../myEngine/lib/bgfx.cmake/bx/include)
#include_directories(../../../../myEngine/lib/bgfx.cmake/bimg/include)


target_link_libraries(${APP_NAME} GameSrc myEngine)

add_dependencies(${APP_NAME} GameSrc myEngine)

# Locate system libraries on iOS
find_library(UIKIT UIKit PATHS ENV PATH)
find_library(FOUNDATION Foundation)
find_library(MOBILECORESERVICES MobileCoreServices)
find_library(CFNETWORK CFNetwork)
find_library(SYSTEMCONFIGURATION SystemConfiguration)

# link the frameworks located above
target_link_libraries(${APP_NAME} ${UIKIT})
target_link_libraries(${APP_NAME} ${FOUNDATION})
target_link_libraries(${APP_NAME} ${MOBILECORESERVICES})
target_link_libraries(${APP_NAME} ${CFNETWORK})
target_link_libraries(${APP_NAME} ${SYSTEMCONFIGURATION})


# Create the app target
set_target_properties(${APP_NAME} PROPERTIES
                      RESOURCE "${RESOURCES}"
                      MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/plist.in
                      XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym"
                      XCODE_ATTRIBUTE_GCC_PREFIX_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/Prefix.pch"
                      XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER "YES"
                      XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET ${DEPLOYMENT_TARGET}
                      XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ${CODE_SIGN_IDENTITY}
                      XCODE_ATTRIBUTE_DEVELOPMENT_TEAM ${DEVELOPMENT_TEAM_ID}
                      XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY ${DEVICE_FAMILY}
                      XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES
                      XCODE_ATTRIBUTE_COMBINE_HIDPI_IMAGES NO
                      XCODE_ATTRIBUTE_INSTALL_PATH "$(LOCAL_APPS_DIR)"
                      XCODE_ATTRIBUTE_ENABLE_TESTABILITY YES
                      XCODE_ATTRIBUTE_GCC_SYMBOLS_PRIVATE_EXTERN YES
)

# Turn on ARC
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fobjc-arc")
