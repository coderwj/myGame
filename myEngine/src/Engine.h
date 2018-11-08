#pragma once

#include <string>
#include <vector>
#include <unordered_map>

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

	private:
		static Engine* e;

		Engine();
		~Engine();

		Camera* m_maincCamera;

		Renderer* m_renderer;

		std::unordered_map<int, Model*> m_model_map;

	};

}
