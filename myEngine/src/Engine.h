#pragma once

#include <string>
#include <vector>

namespace myEngine
{
	class Camera;
	class Renderer;
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

		void addAnimation(Animation* anim);

	private:
		static Engine* e;

		Engine();
		~Engine();

		Camera* m_maincCamera;

		Renderer* m_renderer;

		std::vector<Animation*> m_animations;
	};

}
