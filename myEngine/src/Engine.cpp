#include "Engine.h"

#include "Camera.h"
#include "Renderer.h"
#include "Model.h"
#include "stb_image.h"
#include "config.h"
#include "StringDef.h"

#include <cassert>

#define CUBE_FACE_NUM 6

namespace myEngine
{
	Engine*  Engine::e = nullptr;

	Engine::Engine()
	:m_maincCamera(nullptr)
	,m_renderer(nullptr)
	,m_totalTime(0)
	{
		m_skyboxTextureCube = BGFX_INVALID_HANDLE;
		m_diffuseEnvTextureCube = BGFX_INVALID_HANDLE;
		m_specularEnvTextureCube = BGFX_INVALID_HANDLE;
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

		_initTextures();
		
		return true;
	}
	
	void Engine::tick(int delta)
	{
		m_totalTime += delta;
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

	void Engine::_initTextures()
	{
		std::string envMapPath = Config::game_res_path + Config::GetConfigStr("envmap_path");
		char buff[64];

		std::vector<std::string> diffuseTextures;
		for (int i = 0; i < CUBE_FACE_NUM; i++)
		{
			myEngine::sprintf(buff, 64, "cubemap_%d", i);
			std::string faceStr = Config::GetConfigStr(buff);
			myEngine::sprintf(buff, 64, "diffuse_%s_0.jpg", faceStr.c_str());
			diffuseTextures.push_back(envMapPath + buff);
		}
		m_diffuseEnvTextureCube = _loadCubemap(diffuseTextures);


		std::vector<std::string> specularTextures;
		const int _mips = ::atoi(Config::GetConfigStr("envmap_specular_mips").c_str());
		for (int j = 0; j < _mips; j++)
		{
			for (int i = 0; i < CUBE_FACE_NUM; i++)
			{
				myEngine::sprintf(buff, 64, "cubemap_%d", i);
				std::string faceStr = Config::GetConfigStr(buff);
				myEngine::sprintf(buff, 64, "specular_%s_%d.jpg", faceStr.c_str(), j);
				specularTextures.push_back(envMapPath + buff);
			}
		}
		m_specularEnvTextureCube = _loadCubemap(specularTextures);

		const std::string& brdfLUTName = Config::GetConfigStr("brdfLUT_texture");
		m_brdfLUTTexture = _loadTexture(envMapPath + brdfLUTName);
	}

	bgfx::TextureHandle Engine::_loadCubemap(const std::vector<std::string>& faces)
	{
		assert(faces.size() % CUBE_FACE_NUM == 0);

		const int MIP_NUM = faces.size() / CUBE_FACE_NUM;

		std::vector<int> width(MIP_NUM);
		std::vector<int> height(MIP_NUM);
		int component;
		std::vector<unsigned char*> imageData;
		imageData.reserve(faces.size());
		for (size_t i = 0; i < faces.size(); i++)
		{
			unsigned char* _data = stbi_load(faces[i].c_str(), &width[i / CUBE_FACE_NUM], &height[i / CUBE_FACE_NUM], &component, STBI_default);
			imageData.push_back(_data);
		}

		bgfx::TextureFormat::Enum _format;
		if (component == 3)
			_format = bgfx::TextureFormat::RGB8;
		else if (component == 4)
			_format = bgfx::TextureFormat::RGBA8;
		else
			_format = bgfx::TextureFormat::Count;

		bgfx::TextureHandle _th = bgfx::createTextureCube(static_cast<uint16_t>(width[0]), MIP_NUM > 1, 1, _format, 0, nullptr);

		for (uint8_t i = 0; i < MIP_NUM; i++)
		{
			for (uint8_t j = 0; j < CUBE_FACE_NUM; j++)
			{
				uint32_t _size = static_cast<uint32_t>(width[i] * height[i] * component);
				const bgfx::Memory* _data = bgfx::makeRef(imageData[i * 6 + j], _size);
				bgfx::updateTextureCube(
					_th, 
					1, 
					j, 
					i,
					0,
					0,
					static_cast<uint16_t>(width[i]),
					static_cast<uint16_t>(height[i]),
					_data);
			}
		}

		return _th;
	}

	bgfx::TextureHandle Engine::_loadTexture(std::string path)
	{
		int width, height, component;
		unsigned char* imageData = stbi_load(path.c_str(), &width, &height, &component, STBI_default);

		uint32_t _size = static_cast<uint32_t>(width * height * component);
		const bgfx::Memory* _data = bgfx::makeRef(imageData, _size);

		bgfx::TextureFormat::Enum _format;
		if (component == 3)
			_format = bgfx::TextureFormat::RGB8;
		else if (component == 4)
			_format = bgfx::TextureFormat::RGBA8;
		else
			_format = bgfx::TextureFormat::Count;

		unsigned long long _flags = 0;
		//_flags |= BGFX_TEXTURE_SRGB;

		bgfx::TextureHandle _th = bgfx::createTexture2D(
			static_cast<uint16_t>(width),
			static_cast<uint16_t>(height),
			false,
			1,
			_format,
			_flags,
			_data);

		return _th;
	}

}
