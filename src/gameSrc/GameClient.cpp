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
		string luafile = Config::lua_path + "dofile.lua";
		lua_tinker::dofile(m_state, luafile.c_str());
		lua_tinker::call<void>(m_state, "LuaGameMgr", "InitGame");

		//m_gameScene = GameScene::getInstance();
		//m_gameScene->init();
		//m_gameScene->loadScene("scene_3");

	    m_mainCharacter = Character::Create("model_3");
	    m_mainCharacter->setPosition(Vector3(20.0f, 1.0f, 10.0f));
	
	    Character * character = Character::Create("model_4");
	    character->setPosition(Vector3(18.0f, 1.0f, 11.0f));

	    m_characters.push_back(character);
		m_cameraOption = new CameraOption();
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

	}

	void GameClient::handleTouchBegin(int x, int y)
	{
	}

	void GameClient::handleTouchMove(int x, int y)
	{
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

	void GameClient::onMouseScroll(float offset) const
	{
		if (nullptr != m_cameraOption)
			m_cameraOption->processMouseScroll(offset);
	}

	int GameClient::getFps() const
	{
		return m_fps;
	}

	void GameClient::setFps(int val)
	{
		m_fps = val;
	}
	

}