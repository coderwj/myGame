#include "GameClient.h"

#include <string>
#include <vector>
#include <cmath>
#define _USE_MATH_DEFINES
#include <thread>
#include <sstream>
#include <assert.h>

#include "Character.h"
#include "CameraOption.h"
#include "Config.h"

//#include "luaClientPort.h"
#include "tinyxml2.h"

#include "Engine.h"
#include "HelperFunc.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};
#include "lua_tinker.h"


#ifdef WIN32
#include <Windows.h>
#define sleepFunction(t) Sleep(t)
#else
#include <unistd.h>
#define sleepFunction(t) usleep(t)
#endif

using namespace std;
using namespace myEngine;

namespace myGame
{
	//static data.
	GameClient * GameClient::m_pGameClient = NULL;

	//static functions.
	GameClient * GameClient::getInstance()
	{
		if (m_pGameClient == NULL)
			m_pGameClient = new GameClient();
		return m_pGameClient;
	}

	void GameClient::destroyInstance()
	{
		if (m_pGameClient)
		{
			m_pGameClient->onDestroy();
			delete(m_pGameClient);
			m_pGameClient = NULL;
		}
	}

	GameClient::GameClient() :
		m_gameScene(nullptr),
		m_mainCharacter(nullptr),
		m_cameraOption(nullptr),
		m_state(nullptr),
		m_nowTime(0),
		m_fps(30)
	{
	}

	GameClient::~GameClient()
	{
	}

	void GameClient::onDestroy()
	{
		m_gameScene = nullptr;

		if (m_mainCharacter)
		{
			delete(m_mainCharacter);
			m_mainCharacter = nullptr;
		}
		if (m_cameraOption)
		{
			delete(m_cameraOption);
			m_cameraOption = nullptr;
		}
		m_state = nullptr;

		for (vector<Character *>::iterator it = m_characters.begin(); it != m_characters.end();)
		{
			if (*it)
			{
				delete(*it);
				it = m_characters.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
	
	
	bool GameClient::init(){

		//init client config.
		Config::InitConfigMap();

		//init Engine.
		Engine* pEngine = Engine::getInstance();
		assert(nullptr != pEngine);
		pEngine->init();
	
		//init lua.
		m_state = luaL_newstate();
		luaopen_base(m_state);
		luaL_openlibs(m_state);
		//tolua__open(m_state);
        
        lua_getglobal(m_state, "package");                              /* stack: package */
        lua_getfield(m_state, -1, "path");            /* get package.path, stack: package path */
        const char* cur_path =  lua_tostring(m_state, -1);
        lua_pop(m_state, 1);                                            /* stack: package */
        lua_pushfstring(m_state, "%s;%s?.lua", cur_path, Config::lua_path.c_str());        /* stack: package newpath */
        lua_setfield(m_state, -2, "path");      /* package.path = newpath, stack: package */
        lua_pop(m_state, 1);
        
		string luafile = Config::lua_path + "dofile.lua";
		lua_tinker::dofile(m_state, luafile.c_str());
		lua_tinker::call<void>(m_state, "LuaGameMgr", "InitGame");

//        m_mainCharacter = Character::Create("model_5");
//        m_mainCharacter->setPosition(Vector3(20.0f, 1.0f, 10.0f));
        
        Character* c1 = Character::Create("model_3");
        m_characters.push_back(c1);
        Character* c2 = Character::Create("model_5");
        m_characters.push_back(c2);
        Character* c3 = Character::Create("model_6");
        m_characters.push_back(c3);

		m_cameraOption = new CameraOption();
		m_cameraOption->setCamera(pEngine->getMaincCamera());
		m_cameraOption->resetCameraPos();

	    return true;
	}

	void GameClient::tick()
	{
		if (m_nowTime == 0)
			m_nowTime = HelperFunc::GetCurrentTimeMs();

		//begin tick, get time.
		long long new_time = HelperFunc::GetCurrentTimeMs();

		m_deltaTime = new_time - m_nowTime;
		m_nowTime = new_time;

		_tick(static_cast<int>(m_deltaTime));

		Engine* pEngine = Engine::getInstance();
		if (pEngine)
			pEngine->render();

		//end tick, get time.
		long long runTime = HelperFunc::GetCurrentTimeMs() - m_nowTime;
		if (runTime < static_cast<long long>(1000 / getFps()))
		{
			long long sleepTime = static_cast<long long>(1000 / getFps()) - runTime;
			sleepFunction(static_cast<unsigned long>(sleepTime));
		}
	}

	void GameClient::_tick(int delta)
	{
		//lua_tinker::call<void>(m_state, "LuaGameMgr", "Tick", delta);
		//m_gameScene->tick(delta);
		Engine* pEngine = Engine::getInstance();
		if (pEngine)
			pEngine->tick(delta);
		if (m_mainCharacter)
			m_mainCharacter->tick(delta);

	}

	void GameClient::handleCharInput(int key)
	{

	}

	void GameClient::handleKeyDown(int key)
	{

	}

	void GameClient::handleKeyUp(int key)
	{

	}

	void GameClient::handleMouseWheel(bool wheel_down, int scroll_delta, int x, int y)
	{
		if (nullptr != m_cameraOption)
			m_cameraOption->processMouseScroll(scroll_delta);
	}

	void GameClient::handleTouchBegin(int x, int y)
	{
		m_touchBeginPosX = x;
		m_touchBeginPosY = y;
		m_touchPosX = x;
		m_touchPosY = y;
	}

	void GameClient::handleTouchMove(int x, int y)
	{
		if (nullptr == m_cameraOption)
			return;
		int dx = x - m_touchPosX;
		m_cameraOption->processKeyboard(CameraMoveDir::ROTATERIGHT, dx);
		int dy = y - m_touchPosY;
		m_cameraOption->processKeyboard(CameraMoveDir::ROTATEDOWN, dy);
		m_touchPosX = x;
		m_touchPosY = y;
	}

	void GameClient::handleTouchEnd(int x, int y)
	{
	}

	void GameClient::onResize(int width, int height)
	{
		Engine* pEngine = Engine::getInstance();
		if (nullptr != pEngine)
		{
			pEngine->onResize(width, height);
		}
	}

	int GameClient::getFps() const
	{
		return m_fps;
	}

	void GameClient::setFps(int val)
	{
		m_fps = val;
	}
    
    void GameClient::setResPath(const char* path)
    {
        m_resPath = path;
        myEngine::Config::engine_res_path = m_resPath;
        myEngine::Config::game_res_path = m_resPath;
        Config::shader_bin_path = Config::engine_res_path + "shaderbin/";
        Config::shader_src_path = Config::engine_res_path + "shader/";
        Config::model_path = Config::game_res_path + "models/";
        Config::scene_path = Config::game_res_path + "scenes/";
        Config::lua_path = Config::game_res_path + "luafiles/";
    }
	

}
