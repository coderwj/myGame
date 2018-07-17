#include "GameClient.h"

#include "DebugInfo.h"
#include "luaClientPort.h"
#include "Character.h"
#include "CameraOption.h"
#include "Config.h"

#include "tinyxml2.h"
#include "FbxSdkHelper.h"

#include "Engine.h"
#include "SkyBox.h"
#include "Camera.h"
#include "Shader.h"
#include "Model.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};
#include "lua_tinker.h"

#include <string>
#include <vector>
#include <cmath>
#define _USE_MATH_DEFINES
#include <thread>
#include <sstream>
#include <assert.h>

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
		m_mainCharacter(nullptr),
		m_cameraOption(nullptr),
		m_state(nullptr),
		m_nowTime(0)
	{
	}

	GameClient::~GameClient()
	{
	}

	void GameClient::onDestroy()
	{
		if (m_mainCharacter)
		{
			delete(m_mainCharacter);
			m_mainCharacter = NULL;
		}
		if (m_camera)
		{
			delete(m_camera);
			m_camera = NULL;
		}
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

		Engine* pEngine = Engine::getInstance();
		assert(nullptr != pEngine);
		pEngine->init();

		Config::InitConfigMap();

		DebugInfo * pDebugInfo = DebugInfo::getInstance();
		assert(nullptr != pDebugInfo);
		pDebugInfo->init();
	
		m_state = luaL_newstate();
		luaopen_base(m_state);
		luaL_openlibs(m_state);
		tolua__open(m_state);
		string luafile = Config::lua_path + "dofile.lua";
		lua_tinker::dofile(m_state, luafile.c_str());
	
		lua_tinker::call<void>(m_state, "LuaGameMgr", "InitGame");
	
		if (Config::GetIsShaderTest())
		{
			return true;
		}
	    string fbxFile = Config::model_path + "character2/Maskboy.FBX";
	    printFbxFileData(fbxFile.c_str());
	
		m_skyBox = new SkyBox;
		m_skyBox->init("2/darkcity_");
	
	    string sceneName = "scene_3";
	    loadScene(sceneName.c_str());
	    m_mainCharacter = Character::Create("model_3");
	    m_mainCharacter->setPosition(Vector3(20.0f, 1.0f, 10.0f));
	
	    Character * character = Character::Create("model_4");
	    character->setPosition(Vector3(18.0f, 1.0f, 11.0f));
	    m_characters.push_back(character);
	    if(nullptr == m_mainCharacter)
	        return false;

		m_cameraOption = new CameraOption();
		resetCameraPos();
	    //glDeleteVertexArrays(1, &vao);
	    //glDeleteBuffers(1, &vbo);
	
	    return true;
	}

	void GameClient::tick(float delta)
	{
		m_deltaTime = delta;
		lua_tinker::call<void>(m_state, "LuaGameMgr", "Tick", delta);
		m_nowTime += delta;
	}

	void GameClient::processMouseScroll(float offset) const
	{
		if (nullptr != m_cameraOption)
			m_cameraOption->processMouseScroll(offset);
	}
	

}