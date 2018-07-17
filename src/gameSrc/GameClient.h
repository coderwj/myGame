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

	public:
		//static functions.
		static GameClient *		getInstance();
		static void				destroyInstance();

	public:
		//member functions.
		bool					init					();
		void					tick					(float delta);

		Character*				getMainCharacter		() const { return m_mainCharacter; }
		float					getDeltaTime			() const { return m_deltaTime; }

		myGame::CameraOption*	getCameraOption			() const { return m_cameraOption; }
		void					setCameraOption			(myGame::CameraOption* val) { m_cameraOption = val; }

		void					processMouseScroll		(float offset) const;

	private:
		GameClient();
		~GameClient();
		void					onDestroy				();

	private:
		static GameClient*	m_pGameClient;

		Character*			m_mainCharacter;
		CameraOption*		m_cameraOption;

		std::vector<Character*> m_characters;

		struct lua_State*	m_state;

		float				m_deltaTime;
		float				m_nowTime;

	};
}

