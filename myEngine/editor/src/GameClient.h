#pragma once

#include "Vector3.h"

#include <string>
#include <map>

using namespace myEngine;

struct lua_State;

namespace myEngine
{
    class Camera;
    class Model;
    class Shader;
    class SkyBox;
}

namespace myGame
{
    class Character;
    class CameraOption;
    class MyImguiContext;
    
    class GameClient{

    public:
        //static functions.
        static GameClient *     getInstance();
        static void             destroyInstance();

    public:
        //member functions.
        bool                    init                    ();
        void                    tick                    ();

        void                    handleCharInput         (int key);
        void                    handleKeyDown           (int key);
        void                    handleKeyUp             (int key);
        void                    handleMouseWheel        (bool wheel_down, int scroll_delta, int x, int y);
        void                    handleMouseDown         (int x, int y);
        void                    handleMouseDragged      (int x, int y);
        void                    handleMouseUp           (int x, int y);

        Character*              getMainCharacter        () const { return m_mainCharacter; }
        //float                 getDeltaTime            () const { return m_deltaTime; }

        myGame::CameraOption*   getCameraOption         () const { return m_cameraOption; }
        void                    setCameraOption         (myGame::CameraOption* val) { m_cameraOption = val; }

        void                    onResize                (int width, int height);

        int                     getFps                  () const;
        void                    setFps                  (int val);

    private:
        GameClient();
        ~GameClient();
        void                    onDestroy               ();

        void                    _tick                   (int delta); //real tick(delta ms)

    private:
        static GameClient*  m_pGameClient;

        Character*          m_mainCharacter;
        CameraOption*       m_cameraOption;

        std::map<std::string, Character*> m_characters;

        struct lua_State*   m_state;

        long long           m_deltaTime;
        long long           m_nowTime;

        int                 m_fps;
        int                 m_touchBeginPosX;
        int                 m_touchBeginPosY;
        int                 m_touchPosX;
        int                 m_touchPosY;

        int                 m_windowWidth;
        int                 m_windowHeight;
        
        MyImguiContext*     m_imguiContext;
    };
}

