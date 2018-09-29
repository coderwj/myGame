#pragma once

#include <string>
#include <vector>

#include "Vector3.h"

namespace myEngine
{
	class Model;
	class SkyBox;

	struct FogPara {
		Vector3 color;
		float	start;
		float	end;
		float	intensity;
	};

	class Scene {

	public:

		Scene();
		~Scene();

		bool		init();
		void		render();
		void		tick(float delta);

		void		loadScene(const char* sceneName);
		void		changeScene(const char* sceneName);

	private:

		void onDestroy();
		void _renderScene();

	};
}
