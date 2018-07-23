#pragma once

#include "Vector3.h"

#include <string>
#include <vector>

using namespace myEngine;

namespace myEngine
{
	class Model;
	class SkyBox;
}

namespace myGame
{	
	struct FogPara{
		Vector3 color;
		float	start;
		float	end;
		float	intensity;
	};
	
	class GameScene{

	public:
		static GameScene * getInstance();
		static void destroyInstance();
	
		bool		init();
		void		render();
		void		tick(float delta);
	
		void		loadScene(const char* sceneName);
		void		changeScene(const char* sceneName);

	private:
		static GameScene*	m_pGameScene;

		SkyBox*				m_skyBox;

		Model*				m_model;	//scene model
		float				m_scale;
		float				m_theta;
		Vector3				m_rotateVec;

		struct FogPara		m_fogpara;

		GameScene();
		~GameScene();

		void onDestroy();
		void _renderScene();

	};
}

