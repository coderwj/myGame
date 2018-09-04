#include "Engine.h"

#include "Camera.h"
#include "Renderer.h"

namespace myEngine
{
	Engine*  Engine::e = nullptr;

	Engine::Engine()
	:m_maincCamera(nullptr)
	,m_renderer(nullptr)
	{
		onCreate();
	}

	Engine::~Engine()
	{
		onDestroy();
	}

	Engine * Engine::getInstance()
	{
		if (nullptr == e)
			e = new Engine();
		return e;
	}

	void Engine::destroyInstance() {
		if (e)
		{
			delete(e);
			e = nullptr;
		}
	}

	void Engine::onCreate()
	{

	}

	void Engine::onDestroy()
	{

	}

	void Engine::onResize(int width, int height)
	{
		if (nullptr != m_renderer)
		{
			m_renderer->setViewPort(width, height);
		}
		if (nullptr != m_maincCamera)
		{
			m_maincCamera->setAspect(static_cast<float>(width) / static_cast<float>(height));
			m_maincCamera->setViewPortWidth(static_cast<float>(width));
		}
	}

	bool Engine::init()
	{
		m_renderer = Renderer::getInstance();
		m_renderer->init();

		m_maincCamera = new Camera();
		return true;
	}
	
	void Engine::tick(float delta)
	{

	}
	void Engine::render()
	{
		m_renderer->render();
	}
}
