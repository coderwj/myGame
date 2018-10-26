#include "Engine.h"

#include "Camera.h"
#include "Renderer.h"
#include "Animation.h"

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
        m_renderer = Renderer::getInstance();
        m_maincCamera = new Camera();
	}

	void Engine::onDestroy()
	{
        if (m_maincCamera)
        {
            delete(m_maincCamera);
            m_maincCamera = nullptr;
        }
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
        if (nullptr != m_renderer)
        {
		    m_renderer->init();
        }

        if (nullptr != m_maincCamera)
        {
            m_maincCamera->setPosition(Vector3(0.f, 0.f, 1.f));
            m_maincCamera->SetFocusPos(Vector3(0.f, 0.f, 0.f));
        }
		return true;
	}
	
	void Engine::tick(int delta)
	{
		for (Animation * anim : m_animations)
		{
			anim->tick(delta);
		}
	}
	void Engine::render()
	{
		m_renderer->render();
	}

	void Engine::addAnimation(Animation * anim)
	{
		m_animations.push_back(anim);
	}

}
