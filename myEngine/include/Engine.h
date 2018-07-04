#pragma once

#include <string>
using namespace std;

namespace myEngine
{
	class Camera;

	static const string engine_res_path;
	static const string model_path;
	static const string scene_path;
	static const string lua_path;

	class Engine
	{
	private:
		static Engine* e;

		Engine();
		~Engine();

		Camera* m_maincCamera;
	public:
		static Engine * getInstance()
		{
			if (nullptr == e)
				e = new Engine();
			return e;
		}
		static void destroyInstance(){
			if(e)
			{
				delete(e);
				e = nullptr;
			}
		}

		void onCreate();
		void onDestroy();
	
		bool init();
		void tick(float delta);
		void render();


		myEngine::Camera* getMaincCamera() const { return m_maincCamera; }
		void setMaincCamera(myEngine::Camera* val) { m_maincCamera = val; }
	};
}
