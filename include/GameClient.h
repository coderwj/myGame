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
	class CameraOption;
	
	class GameClient{
	private:
		static GameClient*	m_pGameClient;
	
		Character*			m_mainCharacter;
		CameraOption*		m_cameraOption;
	
		std::vector<Character*> m_characters;
	
		struct lua_State*	m_state;
	
		float				m_deltaTime;
		float				m_nowTime;
	
		GameClient():
			m_mainCharacter(nullptr),
			m_cameraOption(nullptr),
	        m_state(nullptr),
			m_nowTime(0)
		{
		}
	
		~GameClient(){}

		void onDestroy();

	public:
		static GameClient * getInstance(){
			if(m_pGameClient == NULL)
				m_pGameClient = new GameClient();
			return m_pGameClient;
		}
	
		static void destroyInstance(){
			if(m_pGameClient)
			{
				m_pGameClient->onDestroy();
				delete(m_pGameClient);
				m_pGameClient = NULL;
			}
		}
	
		bool		init();
		void		tick(float delta);

		Character*	getMainCharacter() { return m_mainCharacter; }
		float		getDeltaTime() { return m_deltaTime; }

		myGame::CameraOption* getCameraOption() const { return m_cameraOption; }
		void setCameraOption(myGame::CameraOption* val) { m_cameraOption = val; }
	};
}

