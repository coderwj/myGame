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
	class GameScene;
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
		void					tick					();

		void					handleCharInput			(int key);
		void					handleKeyDown			(int key);
		void					handleKeyUp				(int key);
		void					handleMouseWheel		(bool wheel_down, int scroll_delta, int x, int y);
		void					handleTouchBegin		(int x, int y);
		void					handleTouchMove			(int x, int y);
		void					handleTouchEnd			(int x, int y);

		Character*				getMainCharacter		() const { return m_mainCharacter; }
		//float					getDeltaTime			() const { return m_deltaTime; }

		myGame::CameraOption*	getCameraOption			() const { return m_cameraOption; }
		void					setCameraOption			(myGame::CameraOption* val) { m_cameraOption = val; }

		void					onResize				(int width, int height);

		int						getFps					() const;
		void					setFps					(int val);

	private:
		GameClient();
		~GameClient();
		void					onDestroy				();

		void					_tick					(int delta); //real tick(delta ms)

	private:
		static GameClient*	m_pGameClient;

		GameScene*			m_gameScene;
		Character*			m_mainCharacter;
		CameraOption*		m_cameraOption;

		std::vector<Character*> m_characters;

		struct lua_State*	m_state;

		long long			m_deltaTime;
		long long			m_nowTime;

		int					m_fps;

	};
}

