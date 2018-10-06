
#include <android/native_activity.h>
#include <android_native_app_glue.h>
#include "GameClient.h"
#include "bgfx/platform.h"

struct android_app* g_app;

void androidSetWindow(::ANativeWindow* _window)
{
    bgfx::PlatformData pd;
    pd.ndt          = NULL;
    pd.nwh          = _window;
    pd.context      = NULL;
    pd.backBuffer   = NULL;
    pd.backBufferDS = NULL;
    bgfx::setPlatformData(pd);
}

void onAppCommand(struct android_app *app, int32_t cmd)
{

    switch (cmd)
    {
        case APP_CMD_INIT_WINDOW:
            break;
        case APP_CMD_TERM_WINDOW:
            break;
        default:
            break;
    }
}

int32_t onInputEvent(struct android_app *app, AInputEvent *inputEvent)
{
    if (AInputEvent_getType(inputEvent) == AINPUT_EVENT_TYPE_KEY)
    {
        if (AKeyEvent_getKeyCode(inputEvent) == AKEYCODE_BACK)
        {
            ANativeActivity_finish(app->activity);
        }
        return 1;
    }

    if (AInputEvent_getType(inputEvent) == AINPUT_EVENT_TYPE_MOTION)
    {
        // TODO: Handle touch event...
        return 1;
    }
    return 0;
}

void mainLoop()
{
    volatile bool isDestroyRequested = false;
    while (!isDestroyRequested)
    {
        int events = -1;
        struct android_poll_source *source = nullptr;

        while (ALooper_pollAll(0, nullptr, &events, (void **)&source) >= 0)
        {
            if (source != nullptr)
            {
                source->process(g_app, source);
            }

            if (g_app->destroyRequested != 0)
            {
                isDestroyRequested = true;
            }
        }
        myGame::GameClient::getInstance()->tick();
    }
}

void android_main(struct android_app* _app) {
    g_app = _app;
    androidSetWindow(_app->window);
    _app->onAppCmd     = onAppCommand;
    _app->onInputEvent = onInputEvent;
    myGame::GameClient::getInstance()->init();
    mainLoop();
}