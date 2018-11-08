#include "Engine.h"

#include "Camera.h"
#include "Renderer.h"
#include "Model.h"

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
		for (std::pair<int, Model*> _pair : m_model_map)
		{
			if (nullptr != _pair.second)
			{
				_pair.second->tick(delta);
			}
		}
	}
	void Engine::render()
	{
		m_renderer->render();
	}

	int Engine::createModel(std::string path)
	{
		Model* _model = new Model();
		if (nullptr == _model)
		{
			return -1;
		}
		_model->load(path);
		static int idx = 0;
		m_model_map.insert(std::make_pair(idx, _model));
		return idx++;
	}

	void Engine::destroyModelById(int id)
	{
		std::unordered_map<int, Model*>::iterator it = m_model_map.find(id);
		if (it != m_model_map.end())
		{
			Model* m = it->second;
			if (m)
				delete m;
			m_model_map.erase(id);
		}
	}

	Model * Engine::getModelById(int id)
	{
		std::unordered_map<int, Model*>::iterator it = m_model_map.find(id);
		if (it != m_model_map.end())
		{
			return it->second;
		}
		else
		{
			return nullptr;
		}
	}

}
