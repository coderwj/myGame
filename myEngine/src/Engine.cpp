#include "Engine.h"
#include "Camera.h"

namespace myEngine
{
	Engine*  Engine::e = nullptr;

#ifdef WIN32
	const string engine_res_path = "../../../../../myEngine/res/";
	const string game_res_path = "../../../../../res/";
#else
	const string engine_res_path = "../../../../../../myEngine/res/";
	const string game_res_path = "../../../../../../res/";
#endif
	const string model_path = game_res_path + "models/";
	const string scene_path = game_res_path + "scenes/";
	const string lua_path = game_res_path + "luafiles/";

	Engine::Engine()
	:m_maincCamera(nullptr)
	{
		onCreate();
	}

	Engine::~Engine()
	{
		onDestroy();
	}

	void Engine::onCreate()
	{

	}

	void Engine::onDestroy()
	{

	}

	bool Engine::init()
	{
		return true;
	}
	
	void Engine::tick(float delta)
	{

	}
	void Engine::render()
	{

	}
}
