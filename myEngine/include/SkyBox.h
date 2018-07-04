#pragma once

namespace myEngine
{
	class Shader;
	class Vector3;
	class SkyBox
	{
	public:
		SkyBox();
		~SkyBox();

		bool init(const char* modelName);
		void render();
	private:
		myEngine::Shader * m_shader;

		float m_scale;
		float m_theta;		//rotate by Y
		myEngine::Vector3 m_position;//pos in world
	};
}

