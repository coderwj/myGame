#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "bgfx/bgfx.h"

namespace myEngine
{
	class Camera;
	class Renderer;
	class Model;
	class Animation;

	class Engine
	{
	public:
		static Engine * getInstance();
		static void destroyInstance();

		void onCreate();
		void onDestroy();
		void onResize(int width, int height);
	
		bool init();
		void tick(int delta);
		void render();

		myEngine::Camera* getMaincCamera() const { return m_maincCamera; }
		void setMaincCamera(myEngine::Camera* val) { m_maincCamera = val; }

		int createModel(std::string path);
		void destroyModelById(int id);
		Model* getModelById(int id);

		bgfx::TextureHandle getSpecularEnvTextureHandle() { return m_specularEnvTextureCube; }
		bgfx::TextureHandle getDiffuseEnvTextureHandle() { return m_diffuseEnvTextureCube; }
		bgfx::TextureHandle getBrdfLUTTexture() { return m_brdfLUTTexture; }

		int getTotalTime() const { return m_totalTime; }
	private:
		Engine();
		~Engine();

		void _initTextures();

		static bgfx::TextureHandle _loadCubemap(const std::vector<std::string>& faces);
		static bgfx::TextureHandle _loadTexture(std::string path);

	private:
		static Engine* e;

		Renderer* m_renderer;
		Camera* m_maincCamera;

		std::unordered_map<int, Model*> m_model_map;

		bgfx::TextureHandle m_skyboxTextureCube;
		bgfx::TextureHandle m_specularEnvTextureCube;
		bgfx::TextureHandle m_diffuseEnvTextureCube;
		bgfx::TextureHandle m_brdfLUTTexture;

		int m_totalTime;

	};

}
