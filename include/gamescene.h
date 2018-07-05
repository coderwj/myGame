#pragma once

#include "Vector3.h"

#include <string>
#include <vector>

using namespace myEngine;

struct lua_State;

namespace myEngine
{
	class Camera;
	class Model;
	class Shader;
	class SkyBox;
}

namespace myGame
{
	class Character;
	
	struct FogPara{
		Vector3 color;
		float	start;
		float	end;
		float	intensity;
	};
	
	class GameScene{
	private:
		static GameScene*	gs;
	
		Character*			m_mainCharacter;
	
		SkyBox*				m_skyBox;
		Model*				m_model;	//scene model
		Shader*				m_shader;
	
		float				m_scale;
		float				m_theta;
		Vector3				m_rotateVec;
	
		std::vector<Character*> m_characters;
	
		struct lua_State*	m_state;
	
		float				m_deltaTime;
		float				m_nowTime;
	
		struct FogPara		m_fogpara;
	
		GameScene():
			m_mainCharacter(NULL),
			m_scale(1.0f),
			m_theta(0.0f),
	        m_state(NULL),
			m_nowTime(0)
		{
			m_rotateVec = Vector3(0.0f, 1.0f, 0.0f);
			m_fogpara.color = Vector3(1.0f, 1.0f, 1.0f);
			m_fogpara.start = 10.f;
			m_fogpara.end = 100.f;
			m_fogpara.intensity = 1.f;
		}
	
		~GameScene(){}

		void onDestroy();

		void _renderScene();

	public:
		static GameScene * getInstance(){
			if(gs == NULL)
				gs = new GameScene();
			return gs;
		}
	
		static void destroyInstance(){
			if(gs)
			{
				gs->onDestroy();
				delete(gs);
				gs = NULL;
			}
		}
	
		bool		init();
		void		render();
		void		tick(float delta);
	
		void		loadScene(const char* sceneName);
		void		changeScene(const char* sceneName);

		Character*	getMainCharacter() { return m_mainCharacter; }
		float		getDeltaTime() { return m_deltaTime; }

	};
}

