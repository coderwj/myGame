#include "Engine.h"
#include "Camera.h"

namespace myEngine
{
	Engine*  Engine::e = nullptr;

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
