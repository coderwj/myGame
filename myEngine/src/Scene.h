//#pragma once
//
//#include <string>
//#include <vector>
//
//#include "Vector3.h"
//
//namespace myEngine
//{
//	class Model;
//	class SkyBox;
//
//	struct FogPara {
//		Vector3 color;
//		float	start;
//		float	end;
//		float	intensity;
//	};
//
//	class Scene {
//
//	public:
//		static Scene * getInstance();
//		static void destroyInstance();
//
//		bool		init();
//		void		render();
//		void		tick(float delta);
//
//		void		loadScene(const char* sceneName);
//		void		changeScene(const char* sceneName);
//
//	private:
//		static Scene*		m_pScene;
//
//		SkyBox*				m_skyBox;
//
//		Model*				m_model;	//scene model
//		float				m_scale;
//		float				m_theta;
//		Vector3				m_rotateVec;
//
//		struct FogPara		m_fogpara;
//
//		Scene();
//		~Scene();
//
//		void onDestroy();
//		void _renderScene();
//
//	};
//}
