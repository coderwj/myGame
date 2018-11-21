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

		static bgfx::TextureHandle loadCubemap(const std::vector<std::string>& faces);
		static bgfx::TextureHandle loadTexture(std::string path);

		bgfx::TextureHandle getSpecularEnvTextureHandle() { return m_specularEnvTextureCube; }
		bgfx::TextureHandle getDiffuseEnvTextureHandle() { return m_diffuseEnvTextureCube; }
		bgfx::TextureHandle getBrdfLUTTexture() { return m_brdfLUTTexture; }
	private:
		static Engine* e;

		Engine();
		~Engine();

		Camera* m_maincCamera;

		Renderer* m_renderer;

		std::unordered_map<int, Model*> m_model_map;

		bgfx::TextureHandle m_skyboxTextureCube;
		bgfx::TextureHandle m_specularEnvTextureCube;
		bgfx::TextureHandle m_diffuseEnvTextureCube;
		bgfx::TextureHandle m_brdfLUTTexture;

	};

}
