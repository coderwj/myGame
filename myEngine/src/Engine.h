#pragma once

#include <string>
using namespace std;

namespace myEngine
{
	class Camera;
	class Renderer;

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

	private:
		static Engine* e;

		Engine();
		~Engine();

		Camera* m_maincCamera;

		Renderer* m_renderer;
	};

}
