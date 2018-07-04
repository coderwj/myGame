#pragma once

#include <string>
using namespace std;

namespace myEngine
{
	class Camera;

#ifdef WIN32
	const string engine_res_path = "../../../../../myEngine/res/";
	const string game_res_path = "../../../../../res/";
#else
	const string engine_res_path = "../../../../../../myEngine/res/";
	const string game_res_path = "../../../../../../res/";
#endif
	const string model_path = game_res_path + "models/";
	const string scene_path = game_res_path + "scenes/";
	const string lua_path = game_res_path + "luafiles/";

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
